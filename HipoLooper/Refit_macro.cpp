// Refit_macro.cpp
// ROOT macro (compile with ACLiC): root -l -q 'Refit_macro.cpp++("h1,h2")'
// If you want to refit ALL histograms found, pass an empty string: root -l -q 'Refit_macro.cpp++("")'
// Edit the file list + outDir inside `Refit_macro()`.

#include <TAxis.h>
#include <TCanvas.h>
#include <TClass.h>
#include <TF1.h>
#include <TFile.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TH1.h>
#include <TIterator.h>
#include <TKey.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TList.h>
#include <TObject.h>
#include <TPad.h>
#include <TString.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// ---------- helpers ----------

static inline std::string Trim(const std::string& s) {
    auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    auto e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

static std::vector<std::string> SplitCSV(const std::string& csv) {
    std::vector<std::string> out;
    std::string cur;
    std::istringstream iss(csv);
    while (std::getline(iss, cur, ',')) {
        cur = Trim(cur);
        if (!cur.empty()) out.push_back(cur);
    }
    return out;
}

static bool IsWantedHist(const std::string& hname, const std::vector<std::string>& wanted) {
    if (wanted.empty()) return true;  // if not specified, refit all TH1 we encounter
    return std::find(wanted.begin(), wanted.end(), hname) != wanted.end();
}

static std::vector<std::string> ListRootFiles(const std::string& dir, const std::string& mustContain = "") {
    // NOTE: This helper is kept for backwards compatibility, but the main entry point
    // `Refit_macro()` below uses an explicit hard-coded list of ROOT files.
    std::vector<std::string> files;

    TSystemDirectory d("indir", dir.c_str());
    TList* flist = d.GetListOfFiles();
    if (!flist) return files;

    TIter it(flist);
    while (auto* f = (TSystemFile*)it()) {
        TString name = f->GetName();
        if (f->IsDirectory()) continue;

        if (!name.EndsWith(".root")) continue;
        std::string sname = name.Data();
        if (!mustContain.empty() && sname.find(mustContain) == std::string::npos) continue;

        std::string full = dir;
        if (!full.empty() && full.back() != '/') full += "/";
        full += sname;
        files.push_back(full);
    }

    std::sort(files.begin(), files.end());
    return files;
}

static void RemoveExistingFits(TH1* h) {
    if (!h) return;
    auto* lof = h->GetListOfFunctions();
    if (!lof) return;

    // Remove TF1 objects from the list-of-functions
    // (We keep non-fit decorations if they exist, but you can hard-clear if you want.)
    std::vector<TObject*> toRemove;
    TIter it(lof);
    while (TObject* obj = it()) {
        if (obj->InheritsFrom(TF1::Class())) toRemove.push_back(obj);
    }
    for (auto* obj : toRemove) {
        lof->Remove(obj);
        delete obj;  // important: avoid leaking old TF1
    }
}

struct FitSummary {
    double mu = NAN, emu = NAN;
    double sigma = NAN, esigma = NAN;
    double amp = NAN, eamp = NAN;
    double chi2 = NAN;
    int ndf = 0;
    bool ok = false;
};

static FitSummary RefitGaussianPeak(TH1* h, double rangeNSigma = 2.5, double minRangeBins = 6) {
    FitSummary s;
    if (!h) return s;

    // Basic sanity
    if (h->GetEntries() <= 0) return s;
    if (h->GetNbinsX() < 5) return s;

    // Find peak position
    int ibinMax = h->GetMaximumBin();
    double xPeak = h->GetXaxis()->GetBinCenter(ibinMax);

    // Estimate width
    double rms = h->GetRMS();
    if (!(rms > 0)) {
        // fallback: a few bin-widths
        rms = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin()) / h->GetNbinsX();
        rms *= 2.0;
    }

    double xmin = xPeak - rangeNSigma * rms;
    double xmax = xPeak + rangeNSigma * rms;

    // Ensure a minimum range in bins
    double bw = h->GetXaxis()->GetBinWidth(std::max(1, ibinMax));
    double minHalfRange = 0.5 * minRangeBins * bw;
    if ((xmax - xmin) < 2.0 * minHalfRange) {
        xmin = xPeak - minHalfRange;
        xmax = xPeak + minHalfRange;
    }

    // Clamp to axis limits
    xmin = std::max(xmin, h->GetXaxis()->GetXmin());
    xmax = std::min(xmax, h->GetXaxis()->GetXmax());
    if (!(xmax > xmin)) return s;

    // Remove previous fits and refit
    RemoveExistingFits(h);

    // Create a fresh Gaussian (unique name per hist to avoid collisions)
    TString fname = TString::Format("gaus_refit_%s", h->GetName());
    auto* f = new TF1(fname, "gaus", xmin, xmax);

    // Initial parameters: amplitude, mean, sigma
    double amp0 = h->GetBinContent(ibinMax);
    double sig0 = std::max(rms / 2.0, 0.5 * bw);

    f->SetParameters(amp0, xPeak, sig0);

    // Fit quietly, use range, return full result
    // R: use function range
    // Q: quiet
    // 0: do not draw
    // S: return TFitResultPtr
    TFitResultPtr r = h->Fit(f, "RQ0S");

    if ((int)r != 0) {
        // fit failed; keep TF1 around for inspection or delete it
        // Here: keep it attached so you can see what happened.
        return s;
    }

    s.amp = f->GetParameter(0);
    s.mu = f->GetParameter(1);
    s.sigma = f->GetParameter(2);

    s.eamp = f->GetParError(0);
    s.emu = f->GetParError(1);
    s.esigma = f->GetParError(2);

    s.chi2 = r->Chi2();
    s.ndf = r->Ndf();
    s.ok = true;

    // Attach function to histogram (ROOT does this, but we ensure it remains)
    // (No need to Add explicitly; Fit already adds it.)
    return s;
}

static void UpdateLegendForHist(TLegend* leg, TH1* h, const FitSummary& fs) {
    if (!leg || !h) return;

    // Find existing legend entry that corresponds to THIS histogram
    TLegendEntry* histEntry = nullptr;

    TIter it(leg->GetListOfPrimitives());
    while (TObject* obj = it()) {
        auto* e = dynamic_cast<TLegendEntry*>(obj);
        if (!e) continue;
        if (e->GetObject() == h) {
            histEntry = e;
            break;
        }
    }

    // If none exists, create one (but you said it already exists)
    if (!histEntry) { histEntry = leg->AddEntry(h, h->GetTitle(), "l"); }

    // Compose new label:
    // - "Peak" (we use fit mean and its error)
    // - sigma ± error
    // - plus fit quality (chi2/ndf)
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);

    if (fs.ok) {
        ss << "Peak = " << fs.mu << " #pm " << fs.emu << ", #sigma = " << fs.sigma << " #pm " << fs.esigma;

        if (fs.ndf > 0) { ss << ", #chi^{2}/ndf = " << std::setprecision(2) << (fs.chi2 / fs.ndf); }
    } else {
        ss << "Peak fit failed";
    }

    histEntry->SetLabel(ss.str().c_str());

    // OPTIONAL: also ensure a fit entry exists in the legend (separate line).
    // If you prefer a separate entry for the fit curve, uncomment below.

    /*
    // Try to find entry for the TF1 attached to histogram (last TF1 in list)
    TF1* lastF = nullptr;
    if (auto* lof = h->GetListOfFunctions()) {
      TIter fitIt(lof);
      while (TObject* o = fitIt()) {
        if (o->InheritsFrom(TF1::Class())) lastF = (TF1*)o;
      }
    }
    if (lastF) {
      // Look for an existing entry for lastF
      TLegendEntry* fitEntry = nullptr;
      TIter it2(leg->GetListOfPrimitives());
      while (TObject* obj = it2()) {
        auto* e = dynamic_cast<TLegendEntry*>(obj);
        if (!e) continue;
        if (e->GetObject() == lastF) { fitEntry = e; break; }
      }
      if (!fitEntry) {
        fitEntry = leg->AddEntry(lastF, "Gaussian fit", "l");
      }
      std::ostringstream sf;
      sf << std::fixed << std::setprecision(3);
      if (fs.ok) {
        sf << "Fit: #mu=" << fs.mu << "#pm" << fs.emu
           << ", #sigma=" << fs.sigma << "#pm" << fs.esigma;
      } else {
        sf << "Fit: failed";
      }
      fitEntry->SetLabel(sf.str().c_str());
    }
    */
}

static std::string BaseNameNoExt(const std::string& path) {
    TString p(path.c_str());
    TString b = gSystem->BaseName(p);
    std::string s = b.Data();
    if (s.size() >= 5 && s.substr(s.size() - 5) == ".root") s = s.substr(0, s.size() - 5);
    return s;
}

static void EnsureDir(const std::string& outDir) {
    if (outDir.empty()) return;
    gSystem->mkdir(outDir.c_str(), true);
}

static void SaveCanvasPDF(TCanvas* c, const std::string& outDir, const std::string& tag) {
    if (!c) return;
    EnsureDir(outDir);
    std::string pdf = outDir;
    if (!pdf.empty() && pdf.back() != '/') pdf += "/";
    pdf += tag;
    pdf += ".pdf";
    c->SaveAs(pdf.c_str());
}

static void SaveHistPDF(TH1* h, const std::string& outDir, const std::string& tag) {
    if (!h) return;
    EnsureDir(outDir);

    // Draw histogram on a temporary canvas and save it
    TString cname = TString::Format("c_tmp_%s", h->GetName());
    std::unique_ptr<TCanvas> c(new TCanvas(cname, cname, 900, 700));
    c->cd();
    h->Draw(h->GetDrawOption());

    // If there is a fit attached, draw it too (Fit already adds it to the function list)
    c->Modified();
    c->Update();

    std::string pdf = outDir;
    if (!pdf.empty() && pdf.back() != '/') pdf += "/";
    pdf += tag;
    pdf += ".pdf";
    c->SaveAs(pdf.c_str());
}

static void ProcessCanvas(TCanvas* c, const std::vector<std::string>& wantedHists, double rangeNSigma, double minRangeBins, const std::string& outDir, const std::string& pdfTagPrefix) {
    if (!c) return;

    // Find TH1 and TLegend in the canvas primitives (also search recursively in pads)
    std::vector<TH1*> hists;
    std::vector<TLegend*> legends;

    auto scanList = [&](TList* lst, auto&& scanRef) -> void {
        if (!lst) return;
        TIter it(lst);
        while (TObject* obj = it()) {
            if (obj->InheritsFrom(TH1::Class())) {
                hists.push_back((TH1*)obj);
            } else if (obj->InheritsFrom(TLegend::Class())) {
                legends.push_back((TLegend*)obj);
            } else if (obj->InheritsFrom(TPad::Class())) {
                auto* p = (TPad*)obj;
                scanRef(p->GetListOfPrimitives(), scanRef);
            }
        }
    };

    scanList(c->GetListOfPrimitives(), scanList);

    if (hists.empty()) return;

    // If multiple legends exist, we’ll update them all (safe).
    for (TH1* h : hists) {
        if (!h) continue;

        std::string hname = h->GetName();
        if (!IsWantedHist(hname, wantedHists)) continue;

        FitSummary fs = RefitGaussianPeak(h, rangeNSigma, minRangeBins);

        // Update legend label(s)
        for (TLegend* leg : legends) { UpdateLegendForHist(leg, h, fs); }
    }

    // Redraw once after all updates
    c->cd();
    c->Modified();
    c->Update();

    // Save canvas as PDF
    std::string tag = pdfTagPrefix;
    if (!tag.empty()) tag += "__";
    tag += c->GetName();
    SaveCanvasPDF(c, outDir, tag);
}

static void CopyAndProcessFile(const std::string& inFile, TFile& fout, const std::vector<std::string>& wantedHists, double rangeNSigma, double minRangeBins, const std::string& outDir) {
    TFile fin(inFile.c_str(), "READ");
    if (fin.IsZombie()) {
        std::cerr << "ERROR: cannot open input file: " << inFile << "\n";
        return;
    }

    const std::string base = BaseNameNoExt(inFile);

    // Create a directory inside the output ROOT file for this input file
    fout.cd();
    TDirectory* inDir = fout.GetDirectory(base.c_str());
    if (!inDir) inDir = fout.mkdir(base.c_str());
    if (!inDir) {
        std::cerr << "ERROR: cannot create output subdirectory in ROOT file for: " << base << "\n";
        return;
    }

    // Loop keys; clone objects; process canvases/hists; write to output
    TIter nextKey(fin.GetListOfKeys());
    while (TKey* key = (TKey*)nextKey()) {
        TObject* obj = key->ReadObj();
        if (!obj) continue;

        // If this is a canvas, process it (refit + update legend) and also save PDF
        if (obj->InheritsFrom(TCanvas::Class())) {
            auto* c = (TCanvas*)obj;

            ProcessCanvas(c, wantedHists, rangeNSigma, minRangeBins, outDir, base);

            inDir->cd();
            c->Write(c->GetName(), TObject::kOverwrite);
            delete c;
            continue;
        }

        // If this is a standalone histogram, refit it, save a PDF, and write it
        if (obj->InheritsFrom(TH1::Class())) {
            auto* h = (TH1*)obj;
            std::string hname = h->GetName();

            if (IsWantedHist(hname, wantedHists)) {
                (void)RefitGaussianPeak(h, rangeNSigma, minRangeBins);

                // Save a PDF for this histogram
                std::string tag = base + "__" + hname;
                SaveHistPDF(h, outDir, tag);
            }

            inDir->cd();
            h->Write(h->GetName(), TObject::kOverwrite);
            delete h;
            continue;
        }

        // Default: copy object as-is
        inDir->cd();
        obj->Write(obj->GetName(), TObject::kOverwrite);
        delete obj;
    }

    fin.Close();
}

void RefitAll(const std::vector<std::string>& rootFiles, const char* outDir = "./refit_outputs", const char* wantedHistsCSV = "", double rangeNSigma = 2.5, double minRangeBins = 6.0) {
    std::string sOutDir = outDir ? outDir : "./refit_outputs";
    EnsureDir(sOutDir);

    std::string csv = wantedHistsCSV ? wantedHistsCSV : "";
    std::vector<std::string> wanted = SplitCSV(csv);

    if (rootFiles.empty()) {
        std::cerr << "No input ROOT files provided in the internal list.\n";
        return;
    }

    // Single output ROOT file in the same directory as the PDFs
    std::string outRoot = sOutDir;
    if (!outRoot.empty() && outRoot.back() != '/') outRoot += "/";
    outRoot += "refit_results.root";

    TFile fout(outRoot.c_str(), "RECREATE");
    if (fout.IsZombie()) {
        std::cerr << "ERROR: cannot create output ROOT file: " << outRoot << "\n";
        return;
    }

    for (const auto& inFile : rootFiles) {
        std::cout << "Processing: " << inFile << "\n";
        CopyAndProcessFile(inFile, fout, wanted, rangeNSigma, minRangeBins, sOutDir);
    }

    fout.Close();
    std::cout << "Wrote ROOT output: " << outRoot << "\n";
}

// Convenience wrapper:
// root -l -q 'Refit_macro.cpp++("h1,h2")'
// Output directory is set inside the function below.
int Refit_macro(const char* wantedHistsCSV = "") {
    // --------- USER OUTPUT DIRECTORY (EDIT THIS) ---------
    const char* outDir = "/Users/alon/Downloads";  // PDFs + refit_results.root will be written here
    // -----------------------------------------------------

    // --------- USER INPUT LIST (EDIT THIS) ---------
    // Put absolute or relative paths to the ROOT files you want to process.
    // IMPORTANT: Do NOT wrap paths with extra quotes.
    std::vector<std::string> rootFiles = {
        R"(/Users/alon/Code runs/utils/HipoLooper (Ar40 imp)/22_HipoLooper_v22/22_HipoLooper_v22_C12_data_2GeV_run_015664__redo_full_Vx_Vy_sampling/22_HipoLooper_v22_C12_data_2GeV_run_015664__redo_full_Vx_Vy_sampling.root)",
        R"(/Users/alon/Code runs/utils/HipoLooper (Ar40 imp)/22_HipoLooper_v22/22_HipoLooper_v22_C12_data_4GeV_run_015778__redo_full_Vx_Vy_sampling/22_HipoLooper_v22_C12_data_4GeV_run_015778__redo_full_Vx_Vy_sampling.root)",
        R"(/Users/alon/Code runs/utils/HipoLooper (Ar40 imp)/22_HipoLooper_v22/22_HipoLooper_v22_Ar40_data_2GeV_run_015672__redo_full_Vx_Vy_sampling/22_HipoLooper_v22_Ar40_data_2GeV_run_015672__redo_full_Vx_Vy_sampling.root)",
        R"(/Users/alon/Code runs/utils/HipoLooper (Ar40 imp)/22_HipoLooper_v22/22_HipoLooper_v22_Ar40_data_4GeV_run_015743__redo_full_Vx_Vy_sampling/22_HipoLooper_v22_Ar40_data_4GeV_run_015743__redo_full_Vx_Vy_sampling.root)",
        R"(/Users/alon/Code runs/utils/HipoLooper (Ar40 imp)/22_HipoLooper_v22/22_HipoLooper_v22_Ar40_data_6GeV_run_015792__redo_full_Vx_Vy_sampling/22_HipoLooper_v22_Ar40_data_6GeV_run_015792__redo_full_Vx_Vy_sampling.root)"};
    // ----------------------------------------------

    // Run the refit over the explicit list
    RefitAll(rootFiles, outDir, wantedHistsCSV);
    return 0;
}