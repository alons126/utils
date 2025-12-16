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
#include <TLine.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
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

static std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)std::tolower(c); });
    return s;
}

static bool FindSubstring(const std::string& s, const std::string& sub) {
    // Case-insensitive substring search
    const std::string sl = ToLower(s);
    const std::string subl = ToLower(sub);
    return sl.find(subl) != std::string::npos;
}

static std::string GetCodeRunStatusOrExit(const std::string& inputPath) {
    // Use "data" to identify data files
    bool IsData = FindSubstring(inputPath, "data");

    bool Is2GeV = (FindSubstring(inputPath, "2070MeV") || FindSubstring(inputPath, "2gev") || FindSubstring(inputPath, "2GeV"));
    bool Is4GeV = (FindSubstring(inputPath, "4029MeV") || FindSubstring(inputPath, "4gev") || FindSubstring(inputPath, "4GeV"));
    bool Is6GeV = (FindSubstring(inputPath, "5986MeV") || FindSubstring(inputPath, "6gev") || FindSubstring(inputPath, "6GeV"));

    double Ebeam = Is2GeV ? 2.07052 : Is4GeV ? 4.02962 : Is6GeV ? 5.98636 : 0.0;
    if (Ebeam == 0.0) {
        std::cerr << "\n\nError! Ebeam not found in input path string! Aborting...\n\n";
        std::cerr << "Input path: " << inputPath << "\n";
        exit(1);
    }

    std::string target_status = FindSubstring(inputPath, "C12") ? "C12" : FindSubstring(inputPath, "Ar40") ? "Ar40" : "_Unknown";

    if (target_status == "_Unknown") {
        std::cerr << "\n\nError! Target not found in input path string! Aborting...\n\n";
        std::cerr << "Input path: " << inputPath << "\n";
        exit(1);
    }

    std::string sample_type_status = IsData ? "_data" : "_sim";
    std::string genie_tune_status = !IsData ? "_G18_" : "_";
    std::string Ebeam_status_1 = Is2GeV ? "2GeV" : Is4GeV ? "4GeV" : Is6GeV ? "6GeV" : "_Unknown";

    if (Ebeam_status_1 == "_Unknown") {
        std::cerr << "\n\nError! Ebeam not found in input path string! Aborting...\n\n";
        std::cerr << "Input path: " << inputPath << "\n";
        exit(1);
    }

    std::string Run_status = FindSubstring(inputPath, "015664")   ? "_run_015664"
                             : FindSubstring(inputPath, "015778") ? "_run_015778"
                             : FindSubstring(inputPath, "015672") ? "_run_015672"
                             : FindSubstring(inputPath, "015743") ? "_run_015743"
                             : FindSubstring(inputPath, "015792") ? "_run_015792"
                                                                  : "";

    std::string CodeRun_status = target_status + sample_type_status + genie_tune_status + Ebeam_status_1 + Run_status;
    return CodeRun_status;
}

static std::string JoinPath(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    if (a.back() == '/') return a + b;
    return a + "/" + b;
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

static void DrawAttachedFits(TH1* h) {
    if (!h) return;
    auto* lof = h->GetListOfFunctions();
    if (!lof) return;
    TIter it(lof);
    while (TObject* obj = it()) {
        if (obj->InheritsFrom(TF1::Class())) {
            obj->Draw("same");
        }
    }
}

// ---------- Measured target TLine helpers ----------
static bool IsMeasuredTargetLine(const TLine* l) {
    if (!l) return false;
    // Measured line was saved as: color (kGreen+1), width 3, style 2
    return (l->GetLineColor() == (kGreen + 1)) && (l->GetLineWidth() == 3) && (l->GetLineStyle() == 2);
}

static void RemoveMeasuredTargetLinesFromPad(TPad* pad) {
    if (!pad) return;
    TList* prims = pad->GetListOfPrimitives();
    if (!prims) return;

    std::vector<TObject*> toRemove;
    TIter it(prims);
    while (TObject* obj = it()) {
        if (!obj->InheritsFrom(TLine::Class())) continue;
        auto* l = (TLine*)obj;
        if (IsMeasuredTargetLine(l)) {
            toRemove.push_back(obj);
        }
    }

    for (auto* obj : toRemove) {
        prims->Remove(obj);
        delete obj;
    }
}

static TLine* DrawMeasuredTargetLineAtPeak(TPad* pad, double xPeak) {
    if (!pad) return nullptr;
    pad->cd();
    pad->Update();

    // Use pad coordinates directly to avoid dependency on TFrame
    double y2 = pad->GetUymax();

    auto* l = new TLine(xPeak, 0., xPeak, y2);
    l->SetLineColor(kGreen + 1);
    l->SetLineWidth(3);
    l->SetLineStyle(2);
    l->Draw("same");
    return l;
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
    f->SetLineColor(kViolet);

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

    // Ensure the fit function is attached to the histogram (and owned) so it is written to the output ROOT file.
    // ROOT's Fit() usually attaches it automatically, but we keep this explicit for robustness.
    if (auto* lof = h->GetListOfFunctions()) {
        if (!lof->FindObject(f)) lof->Add(f);
    }

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

static void RemoveLegendsFromPad(TPad* pad) {
    if (!pad) return;
    TList* prims = pad->GetListOfPrimitives();
    if (!prims) return;

    std::vector<TObject*> toRemove;
    TIter it(prims);
    while (TObject* obj = it()) {
        if (obj->InheritsFrom(TLegend::Class())) {
            toRemove.push_back(obj);
        }
    }

    for (auto* obj : toRemove) {
        prims->Remove(obj);
        delete obj;
    }
}

static TLine* FindSpeacTargetLineOnPad(TPad* pad) {
    if (!pad) return nullptr;
    TList* prims = pad->GetListOfPrimitives();
    if (!prims) return nullptr;

    TIter it(prims);
    while (TObject* obj = it()) {
        if (!obj->InheritsFrom(TLine::Class())) continue;
        auto* l = (TLine*)obj;
        // speac_target_location_TLine was saved as blue
        if (l->GetLineColor() == kBlue) {
            return l;
        }
    }
    return nullptr;
}

static TF1* GetAttachedFit(TH1* h) {
    if (!h) return nullptr;
    auto* lof = h->GetListOfFunctions();
    if (!lof) return nullptr;

    TF1* lastF = nullptr;
    TIter it(lof);
    while (TObject* obj = it()) {
        if (obj->InheritsFrom(TF1::Class())) {
            lastF = (TF1*)obj;
        }
    }
    return lastF;
}

static TLegend* CreateNewLegendWithOrder(TPad* pad, TLine* speacLine, TF1* fit, TLine* measuredLine, const FitSummary& fs) {
    if (!pad) return nullptr;
    pad->cd();

    // Place legend in a reasonably safe default spot
    auto* leg = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);

    // 1) speac_target_location_TLine
    if (speacLine) {
        leg->AddEntry(speacLine, "Spec target", "l");
    }

    // 2) fitted curve
    if (fit) {
        leg->AddEntry(fit, "Gaussian fit", "l");
    }

    // 3) measured_target_location_TLine with fit errors
    if (measuredLine) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(3);
        if (fs.ok) {
            ss << "Measured = " << fs.mu << " #pm " << fs.emu;
        } else {
            ss << "Measured = fit failed";
        }
        leg->AddEntry(measuredLine, ss.str().c_str(), "l");
    }

    leg->Draw();
    return leg;
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
    c->Modified();
    c->Update();
    c->SaveAs(pdf.c_str());
}

static void SaveHistPDF(TH1* h, const std::string& outDir, const std::string& tag) {
    if (!h) return;
    EnsureDir(outDir);

    // Draw histogram on a temporary canvas and save it
    TString cname = TString::Format("c_tmp_%s", h->GetName());
    std::unique_ptr<TCanvas> c(new TCanvas(cname, cname, 900, 700));
    c->cd();
    c->SetGrid(1, 1);
    h->Draw(h->GetDrawOption());
    DrawAttachedFits(h);
    c->Modified();
    c->Update();

    std::string pdf = outDir;
    if (!pdf.empty() && pdf.back() != '/') pdf += "/";
    pdf += tag;
    pdf += ".pdf";
    c->SaveAs(pdf.c_str());
}

static void ProcessCanvas(TCanvas* c, const std::vector<std::string>& wantedHists, double rangeNSigma, double minRangeBins, const std::string& outDir,
                          const std::string& pdfTagPrefix) {
    if (!c) return;

    // Grid on the top-level canvas
    c->SetGrid(1, 1);

    struct HistOnPad {
        TH1* h;
        TPad* pad;
    };

    std::vector<HistOnPad> hists;
    std::vector<TPad*> pads;

    auto scanList = [&](TList* lst, TPad* curPad, auto&& scanRef) -> void {
        if (!lst) return;
        TIter it(lst);
        while (TObject* obj = it()) {
            if (obj->InheritsFrom(TH1::Class())) {
                hists.push_back({(TH1*)obj, curPad});
            } else if (obj->InheritsFrom(TPad::Class())) {
                auto* p = (TPad*)obj;
                p->SetGrid(1, 1);
                pads.push_back(p);
                scanRef(p->GetListOfPrimitives(), p, scanRef);
            }
        }
    };

    pads.push_back(c);
    scanList(c->GetListOfPrimitives(), c, scanList);

    // Remove ONLY the old measured target location TLine(s) (green+1, width 3, style 2)
    for (TPad* p : pads) {
        RemoveMeasuredTargetLinesFromPad(p);
    }
    // Remove all legends from pads before refitting
    for (TPad* p : pads) {
        RemoveLegendsFromPad(p);
    }

    if (hists.empty()) return;

    // Refit and redraw
    for (const auto& hp : hists) {
        TH1* h = hp.h;
        TPad* pad = hp.pad ? hp.pad : c;
        if (!h) continue;

        std::string hname = h->GetName();
        if (!IsWantedHist(hname, wantedHists)) continue;

        FitSummary fs = RefitGaussianPeak(h, rangeNSigma, minRangeBins);

        // Always draw in the correct order so lines/legend are visible:
        // 1) histogram
        // 2) fit
        // 3) lines
        // 4) legend
        pad->cd();
        h->Draw(h->GetDrawOption());
        DrawAttachedFits(h);
        pad->Update();

        // Remove any measured lines that might have been re-added by redraws (safety)
        RemoveMeasuredTargetLinesFromPad(pad);

        // Draw the new measured target location line at the *refitted* Gaussian peak (mu)
        TLine* measuredLine = nullptr;
        if (fs.ok) {
            measuredLine = DrawMeasuredTargetLineAtPeak(pad, fs.mu);
        }

        // Find the speac (blue) line and the fit function for legend
        TLine* speacLine = FindSpeacTargetLineOnPad(pad);
        TF1* fit = GetAttachedFit(h);

        // Remove any legends again (in case redraw introduced one)
        RemoveLegendsFromPad(pad);

        // Create a brand new legend in the requested order:
        // (1) speac line, (2) fit curve, (3) measured line with fit errors
        (void)CreateNewLegendWithOrder(pad, speacLine, fit, measuredLine, fs);

        pad->Modified();
        pad->Update();
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

static void CopyAndProcessFile(const std::string& inFile, TFile& fout, const std::vector<std::string>& wantedHists, double rangeNSigma, double minRangeBins,
                               const std::string& outDirForThisFile) {
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

        // REMOVE CANVAS COPYING ENTIRELY: do not process or write canvases to output
        if (obj->InheritsFrom(TCanvas::Class())) {
            delete obj;
            continue;
        }

        // OUTPUT ROOT FILE: ONLY FITTED HISTOGRAMS (strict version)
        if (obj->InheritsFrom(TH1::Class())) {
            auto* h = (TH1*)obj;
            std::string hname = h->GetName();

            // Skip non-requested histograms entirely
            if (!IsWantedHist(hname, wantedHists)) {
                delete h;
                continue;
            }

            FitSummary fs = RefitGaussianPeak(h, rangeNSigma, minRangeBins);

            // Write ONLY the refitted histogram to the ROOT file
            inDir->cd();
            h->Write(h->GetName(), TObject::kOverwrite);

            // ----- Fresh PDF drawing from scratch -----
            TString cname = TString::Format("c_pdf_%s", h->GetName());
            std::unique_ptr<TCanvas> c(new TCanvas(cname, cname, 900, 700));
            c->SetGrid(1, 1);
            c->cd();

            h->Draw(h->GetDrawOption());
            DrawAttachedFits(h);
            c->Update();

            // SPEC target line (blue)
            double spec_x = 0.0;
            if (FindSubstring(inFile, "C12")) spec_x = (2.5 - 3.0);
            else if (FindSubstring(inFile, "Ar40")) spec_x = (-2.5 - 3.0);

            auto* specLine = new TLine(spec_x, 0.0, spec_x, c->GetUymax());
            specLine->SetLineColor(kBlue);
            specLine->Draw("same");

            // MEASURED target line at REFITTED PEAK
            TLine* measLine = nullptr;
            if (fs.ok) {
                measLine = new TLine(fs.mu, 0.0, fs.mu, c->GetUymax());
                measLine->SetLineColor(kGreen + 1);
                measLine->SetLineWidth(3);
                measLine->SetLineStyle(2);
                measLine->Draw("same");
            }

            // ----- New legend (ORDER MATTERS) -----
            auto* leg = new TLegend(0.55, 0.72, 0.88, 0.88);
            leg->SetBorderSize(0);
            leg->SetFillStyle(0);
            leg->SetTextFont(42);

            leg->AddEntry(specLine, "Spec target", "l");
            if (auto* fit = GetAttachedFit(h)) leg->AddEntry(fit, "Gaussian fit", "l");

            if (measLine) {
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(3)
                   << "Measured = " << fs.mu << " #pm " << fs.emu;
                leg->AddEntry(measLine, ss.str().c_str(), "l");
            }

            leg->Draw();

            c->Modified();
            c->Update();

            std::string tag = base + "__" + hname;
            SaveCanvasPDF(c.get(), outDirForThisFile, tag);

            delete h;
            continue;
        }

        // Default: do NOT copy any other objects (strict: only TH1s are written)
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

    const int VarNumber = 1;  // EDIT THIS if you want a different VarNumber
    for (const auto& inFile : rootFiles) {
        std::cout << "Processing: " << inFile << "\n";

        std::string CodeRun_status = GetCodeRunStatusOrExit(inFile);
        std::string perFileDir = JoinPath(sOutDir, std::to_string(VarNumber) + "_" + CodeRun_status);
        EnsureDir(perFileDir);

        // Create a dedicated ROOT output file for this input file inside its output directory
        std::string outRoot = JoinPath(perFileDir, CodeRun_status + "_refit_results.root");
        TFile fout(outRoot.c_str(), "RECREATE");
        if (fout.IsZombie()) {
            std::cerr << "ERROR: cannot create output ROOT file: " << outRoot << "\n";
            continue;
        }

        CopyAndProcessFile(inFile, fout, wanted, rangeNSigma, minRangeBins, perFileDir);

        fout.Close();
        std::cout << "Wrote ROOT output: " << outRoot << "\n";
    }
}

// Convenience wrapper:
// root -l -q 'Refit_macro.cpp++("h1,h2")'
// Output directory is set inside the function below.
int Refit_macro(const char* wantedHistsCSV = "") {
    (void)wantedHistsCSV;
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

    // Run the refit over the explicit list, always with the fixed set of histograms
    const char* wantedHists = "Vz_e_AC_1e_cut,Vz_pipFD_AC_1e_cut,Vz_pimFD_AC_1e_cut,Vz_pipCD_AC_zoomin_1e_cut,Vz_pimCD_AC_zoomin_1e_cut";
    RefitAll(rootFiles, outDir, wantedHists);
    return 0;
}