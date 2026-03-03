//
// Created by Alon Sportes on 03/03/2026.
//

#ifndef HIPOCHAINLOADER_H
#define HIPOCHAINLOADER_H

#include "HipoChainLoader.h"

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

HipoChainLoader::HipoChainLoader(Options opt) : opt_(std::move(opt)) {}

// GetOptions function --------------------------------------------------------------------------------------------------------------------------------------------------

const HipoChainLoader::Options& HipoChainLoader::GetOptions() const { return opt_; }

// Build function -------------------------------------------------------------------------------------------------------------------------------------------------------

HipoChainLoader::Result HipoChainLoader::Build(clas12root::HipoChain& chain, const std::string& glob_pattern, const std::string& SampleName) const {
    const std::vector<std::string> files = ExpandGlobFiles(glob_pattern);
    return BuildFromFiles(chain, files, SampleName);
}

// BuildFromFiles function ----------------------------------------------------------------------------------------------------------------------------------------------

HipoChainLoader::Result HipoChainLoader::BuildFromFiles(clas12root::HipoChain& chain, const std::vector<std::string>& files, const std::string& SampleName) const {
    Result res;
    res.n_globbed = static_cast<int>(files.size());

    if (files.empty()) {
        std::ostringstream oss;
        oss << "HipoChainLoader: no files matched";
        throw std::runtime_error(oss.str());
    }

    if (opt_.print_progress) {
        std::cout << env::SYSTEM_COLOR << "\nHipoChainLoader:" << env::INFO_COLOR << " probing " << res.n_globbed << " file(s)" << env::RESET_COLOR << "\n" << std::flush;
    }

    res.added_files.reserve(files.size());
    res.skipped_files.reserve(files.size());

    int idx = 0;
    for (const auto& f : files) {
        ++idx;

        const bool good = IsGoodHipoFile_ForkGuard(f, opt_.require_positive_records);

        if (good) {
            chain.Add(f);
            res.added_files.push_back(f);
            ++Num_of_good_hipo_files;
            ++Total_num_of_hipo_files;
        } else {
            res.skipped_files.push_back(f);
            ++Num_of_bad_hipo_files;
            ++Total_num_of_hipo_files;
        }

        if (opt_.print_progress && (idx % 200 == 0 || idx == res.n_globbed)) {
            std::cout << env::SYSTEM_COLOR << "\tchecked " << env::RESET_COLOR << idx << env::SYSTEM_COLOR << "/" << env::RESET_COLOR << res.n_globbed << env::SYSTEM_COLOR << " | added "
                      << env::RESET_COLOR << res.added_files.size() << env::SYSTEM_COLOR << " | skipped " << env::RESET_COLOR << res.skipped_files.size() << "\n"
                      << std::flush;
        }
    }

    res.n_added = static_cast<int>(res.added_files.size());
    res.n_skipped = static_cast<int>(res.skipped_files.size());

    chain.SetReaderTags(opt_.reader_tags);
    if (opt_.turn_off_qadb) { chain.db()->turnOffQADB(); }

    if (opt_.log_skipped && !res.skipped_files.empty()) { WriteSkippedLog(res.skipped_files, SampleName); }

    if (opt_.print_skipped && !res.skipped_files.empty()) {
        std::cout << env::SYSTEM_COLOR << "\nHipoChainLoader: skipped " << env::RESET_COLOR << res.n_skipped << env::SYSTEM_COLOR << " bad file(s)\n";
        for (const auto& f : res.skipped_files) { std::cout << env::SYSTEM_COLOR << "  skipped: " << env::RESET_COLOR << f << "\n" << std::flush; }
    }

    std::cout << std::endl;

    if (res.n_added == 0) {
        std::ostringstream oss;
        oss << "HipoChainLoader: after filtering, chain has 0 files.";
        throw std::runtime_error(oss.str());
    }

    return res;
}

// MakeInputGlobsFromList function --------------------------------------------------------------------------------------------------------------------------------------

std::vector<std::string> HipoChainLoader::MakeInputGlobsFromList(const ExperimentParameters& Experiment) {
    // Use the new ExperimentParameters-owned strings
    const std::string& RecoSamplePath = Experiment.GetRecoSamplePath();
    const std::string& ReconHipoDir = Experiment.GetReconHipoDir();
    const std::string InputHipoFiles = Experiment.GetInputFiles();

    // Determine the effective sample type as robustly as possible (copy logic from AddToHipoChainFromList)
    auto infer_sample_type = [&]() -> int {
        if (Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE || Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE ||
            Experiment.GetSampleType() == ExperimentParameters::SampleType::UNIFORM_TYPE) {
            return Experiment.GetSampleType();
        }
        // Fallback: infer from `Experiment.GetSampleName()`
        if (bt::FindSubstring(Experiment.GetSampleName(), "_data_") || bt::FindSubstring(Experiment.GetSampleName(), "_data")) return ExperimentParameters::SampleType::DATA_TYPE;
        if (bt::FindSubstring(Experiment.GetSampleName(), "_simulation_") || bt::FindSubstring(Experiment.GetSampleName(), "GENIE") || bt::FindSubstring(Experiment.GetSampleName(), "G18") ||
            bt::FindSubstring(Experiment.GetSampleName(), "GEM21") || bt::FindSubstring(Experiment.GetSampleName(), "SuSa") || bt::FindSubstring(Experiment.GetSampleName(), "Uniform")) {
            // Note: "Uniform" here is used as a proxy for uniform samples.
            return (bt::FindSubstring(Experiment.GetSampleName(), "Uniform")) ? ExperimentParameters::SampleType::UNIFORM_TYPE : ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE;
        }
        // Fallback: infer from `RecoSamplePath`
        if (bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production") || (bt::FindSubstring(RecoSamplePath, "rg-m") && bt::FindSubstring(RecoSamplePath, "dst")))
            return ExperimentParameters::SampleType::DATA_TYPE;
        if (bt::FindSubstring(RecoSamplePath, "GENIE_Reco_Samples")) return ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE;
        if (bt::FindSubstring(RecoSamplePath, "Uniform")) return ExperimentParameters::SampleType::UNIFORM_TYPE;
        return ExperimentParameters::SampleType::UNKNOWN_TYPE;
    };
    const int effectiveType = infer_sample_type();

    // Helper for building run globs
    auto make_abs_run_glob = [&](const std::string& base, const std::string& run) -> std::string {
        const bool base_has_leading_slash = (!base.empty() && base.front() == '/');
        return std::string(base_has_leading_slash ? "" : "/") + base + "/" + run + "/*.hipo";
    };

    // Simulation-like samples: return the input glob as a single-element vector
    if (effectiveType == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE || effectiveType == ExperimentParameters::SampleType::UNIFORM_TYPE) { return {InputHipoFiles}; }

    // Data samples
    if (effectiveType == ExperimentParameters::SampleType::DATA_TYPE) {
        const std::vector<std::string>* runs = nullptr;
        if (bt::FindSubstring(Experiment.GetSampleName(), "H1_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "H1_data_2070MeV")) {
            runs = &lists::H1_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "D2_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "D2_data_2070MeV")) {
            runs = &lists::D2_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "C12_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "C12_data_2070MeV")) {
            runs = &lists::C12_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_2070MeV")) {
            runs = &lists::Ar40_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "C12_data_4GeV") || bt::FindSubstring(Experiment.GetSampleName(), "C12_data_4029MeV")) {
            runs = &lists::C12_data_4GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_4GeV") || bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_4029MeV")) {
            runs = &lists::Ar40_data_4GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "H1_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "H1_data_5986MeV")) {
            runs = &lists::H1_data_6GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "D2_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "D2_data_5986MeV")) {
            runs = &lists::D2_data_6GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "C12x4_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "C12x4_data_5986MeV")) {
            runs = &lists::C12x4_data_6GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_5986MeV")) {
            runs = &lists::Ar40_data_6GeV_runs;
        }
        if (runs != nullptr && ReconHipoDir == "") {
            std::vector<std::string> globs;
            for (const auto& run : *runs) { globs.push_back(make_abs_run_glob(RecoSamplePath, run)); }
            return globs;
        }
        // Otherwise, fall back to the explicit file/glob passed by the caller.
        return {InputHipoFiles};
    }

    // Unknown/unsupported sample type: fall back to whatever the caller provided.
    return {InputHipoFiles};
}

// BuildFromList function -----------------------------------------------------------------------------------------------------------------------------------------------

HipoChainLoader::Result HipoChainLoader::BuildFromList(clas12root::HipoChain& chain, const ExperimentParameters& Experiment) const {
    // Get the glob patterns
    std::vector<std::string> globs = MakeInputGlobsFromList(Experiment);
    // Expand all globs and concatenate
    std::vector<std::string> files;
    for (const auto& pat : globs) {
        std::vector<std::string> expanded = ExpandGlobFiles(pat);
        files.insert(files.end(), expanded.begin(), expanded.end());
    }
    // Sort and de-duplicate
    std::sort(files.begin(), files.end());
    files.erase(std::unique(files.begin(), files.end()), files.end());
    const std::string logName = Experiment.GetSampleName();
    return BuildFromFiles(chain, files, logName);
}

// BuildPtrFromList function --------------------------------------------------------------------------------------------------------------------------------------------

std::pair<std::unique_ptr<clas12root::HipoChain>, HipoChainLoader::Result> HipoChainLoader::BuildPtrFromList(const ExperimentParameters& Experiment) const {
    auto chain = std::make_unique<clas12root::HipoChain>();
    Result res = BuildFromList(*chain, Experiment);
    return {std::move(chain), std::move(res)};
}

// BuildPtr function ----------------------------------------------------------------------------------------------------------------------------------------------------

std::pair<std::unique_ptr<clas12root::HipoChain>, HipoChainLoader::Result> HipoChainLoader::BuildPtr(const std::string& glob_pattern, const std::string& SampleName) const {
    auto chain = std::make_unique<clas12root::HipoChain>();
    Result res = Build(*chain, glob_pattern, SampleName);
    return {std::move(chain), std::move(res)};
}

// ExpandGlobFiles function ---------------------------------------------------------------------------------------------------------------------------------------------

std::vector<std::string> HipoChainLoader::ExpandGlobFiles(const std::string& pattern) {
    std::vector<std::string> files;

    glob_t glob_result;
    std::memset(&glob_result, 0, sizeof(glob_result));

    const int ret = ::glob(pattern.c_str(), GLOB_TILDE, nullptr, &glob_result);
    if (ret == 0) {
        files.reserve(glob_result.gl_pathc);
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            if (glob_result.gl_pathv[i]) { files.emplace_back(glob_result.gl_pathv[i]); }
        }
    }

    ::globfree(&glob_result);

    std::sort(files.begin(), files.end());
    return files;
}

// IsGoodHipoFile_ForkGuard function ------------------------------------------------------------------------------------------------------------------------------------

bool HipoChainLoader::IsGoodHipoFile_ForkGuard(const std::string& file, bool require_positive_records) {
    const pid_t pid = ::fork();
    if (pid < 0) { return false; }

    if (pid == 0) {
        int code = 1;
        try {
            hipo::reader r;
            r.open(file.c_str());
            const long nrec = r.getNRecords();

            if (require_positive_records) {
                code = (nrec > 0) ? 0 : 1;
            } else {
                code = 0;
            }
        } catch (...) { code = 1; }
        _exit(code);
    }

    int status = 0;
    const pid_t w = ::waitpid(pid, &status, 0);
    if (w < 0) { return false; }

    if (WIFEXITED(status)) { return WEXITSTATUS(status) == 0; }

    return false;
}

// NowString function ---------------------------------------------------------------------------------------------------------------------------------------------------

std::string HipoChainLoader::NowString() {
    using clock = std::chrono::system_clock;
    const auto now = clock::now();
    const std::time_t t = clock::to_time_t(now);

    std::tm tm_buf;
    localtime_r(&t, &tm_buf);

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// WriteSkippedLog function ---------------------------------------------------------------------------------------------------------------------------------------------

void HipoChainLoader::WriteSkippedLog(const std::vector<std::string>& skipped, const std::string& SampleName) const {
    std::ios_base::openmode mode = std::ios::out;
    mode |= (opt_.append_log ? std::ios::app : std::ios::trunc);

    std::ofstream out(opt_.log_path, mode);
    if (!out) {
        std::cerr << "\nHipoChainLoader: could not open log file: " << opt_.log_path << "\n" << std::flush;
        return;
    }

    out << "============================================================\n";
    out << "Timestamp: " << NowString() << "\n";
    if (!SampleName.empty()) { out << "Sample: " << SampleName << "\n"; }
    out << "Skipped bad HIPO files:\n";
    for (const auto& f : skipped) { out << "  " << f << "\n"; }
    out << "\n";
}

// AddToHipoChain function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AddToHipoChain function */
// This is the old function used to add runs to the HipoChain
void HipoChainLoader::AddToHipoChain(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                     const std::string& InputHipoFiles) {
    bool PrintOut = true;

    if (Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE) {
        if (sn == "D2_data_2070MeV") {
            if (ReconHipoDir == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                std::vector<std::string> Runs = {"015567", "015573", "015578", "015583", "015590", "015595", "015602", "015608", "015613", "015618", "015624", "015568", "015574", "015579",
                                                 "015586", "015591", "015598", "015603", "015609", "015614", "015619", "015625", "015569", "015575", "015580", "015587", "015592", "015599",
                                                 "015604", "015610", "015615", "015620", "015626", "015570", "015576", "015581", "015588", "015593", "015600", "015606", "015611", "015616",
                                                 "015622", "015627", "015572", "015577", "015582", "015589", "015594", "015601", "015607", "015612", "015617", "015623"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempInputHipoFiles = "/" + RecoSamplePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempInputHipoFiles.c_str());

                    if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n" << std::flush; }
                }

                if (PrintOut) { std::cout << endl; }
            }
        } else if (sn == "C12x4_data_5986MeV") {
            if (ReconHipoDir == "") {
                /* Data in cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon */
                std::vector<std::string> Runs = {"015186", "015187", "015188", "015189", "015190", "015191", "015192", "015193", "015194", "015196", "015199", "015200", "015202",
                                                 "015203", "015204", "015205", "015206", "015207", "015210", "015212", "015213", "015214", "015215", "015217", "015219", "015220",
                                                 "015221", "015223", "015224", "015225", "015226", "015228", "015234", "015235", "015236", "015238", "015239", "015240", "015241",
                                                 "015242", "015243", "015245", "015246", "015247", "015248", "015249", "015250", "015252", "015253", "015254", "015255", "015257",
                                                 "015258", "015259", "015260", "015261", "015262", "015263", "015264", "015265", "015266", "015269", "015270", "015271", "015272",
                                                 "015273", "015274", "015275", "015278", "015279", "015280", "015282", "015283", "015284", "015286", "015287", "015288", "015289",
                                                 "015290", "015291", "015292", "015293", "015294", "015295", "015296", "015298", "015300", "015301", "015302", "015303", "015304",
                                                 "015305", "015306", "015307", "015308", "015309", "015310", "015311", "015312", "015313", "015314", "015316", "015317"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempInputHipoFiles = "/" + RecoSamplePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempInputHipoFiles.c_str());

                    if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n" << std::flush; }
                }

                if (PrintOut) { std::cout << endl; }
            }
        } else {
            chain.Add(InputHipoFiles.c_str());
        }
    } else if (Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE) {
        chain.Add(InputHipoFiles.c_str());

        if (PrintOut) { std::cout << env::SYSTEM_COLOR << InputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n\n" << std::flush; }
    }
}
#pragma endregion

// AddToHipoChainFromList function --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AddToHipoChainFromList function */
void HipoChainLoader::AddToHipoChainFromList(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath,
                                             const std::string& ReconHipoDir, const std::string& InputHipoFiles) {
    const bool PrintOut = true;

    // Determine the effective sample type as robustly as possible.
    // The legacy implementation keyed off DataSample/SimulationSample, which are not guaranteed to be set in all code paths.
    // Here we prefer the configured `sampleType`, and fall back to inference from `sn` and `RecoSamplePath` if needed.
    auto infer_sample_type = [&](void) -> int {
        if (Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE || Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE ||
            Experiment.GetSampleType() == ExperimentParameters::SampleType::UNIFORM_TYPE) {
            return Experiment.GetSampleType();
        }

        // Fallback: infer from `sn`
        if (bt::FindSubstring(sn, "_data_") || bt::FindSubstring(sn, "_data")) { return ExperimentParameters::SampleType::DATA_TYPE; }
        if (bt::FindSubstring(sn, "_simulation_") || bt::FindSubstring(sn, "GENIE") || bt::FindSubstring(sn, "G18") || bt::FindSubstring(sn, "GEM21") || bt::FindSubstring(sn, "SuSa") ||
            bt::FindSubstring(sn, "Uniform")) {
            // Note: "Uniform" here is used as a proxy for uniform samples.
            return (bt::FindSubstring(sn, "Uniform")) ? ExperimentParameters::SampleType::UNIFORM_TYPE : ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE;
        }

        // Fallback: infer from `RecoSamplePath`
        if (bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production") || (bt::FindSubstring(RecoSamplePath, "rg-m") && bt::FindSubstring(RecoSamplePath, "dst"))) {
            return ExperimentParameters::SampleType::DATA_TYPE;
        }
        if (bt::FindSubstring(RecoSamplePath, "GENIE_Reco_Samples")) { return ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE; }
        if (bt::FindSubstring(RecoSamplePath, "Uniform")) { return ExperimentParameters::SampleType::UNIFORM_TYPE; }

        return ExperimentParameters::SampleType::UNKNOWN_TYPE;
    };

    const int effectiveType = infer_sample_type();

    // Ensure we build the same absolute paths as the legacy code:
    // legacy used "/" + AnalyzeFilePath + "/" + run + "/*.hipo" assuming AnalyzeFilePath had no leading '/'.
    auto make_abs_run_glob = [&](const std::string& base, const std::string& run) -> std::string {
        const bool base_has_leading_slash = (!base.empty() && base.front() == '/');
        return std::string(base_has_leading_slash ? "" : "/") + base + "/" + run + "/*.hipo";
    };

    auto add_runs = [&](const std::vector<std::string>& runs) {
        for (size_t i = 0; i < runs.size(); ++i) {
            const std::string TempInputHipoFiles = make_abs_run_glob(RecoSamplePath, runs.at(i));
            chain.Add(TempInputHipoFiles.c_str());
            if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n" << std::flush; }
        }
        if (PrintOut) { std::cout << endl; }
    };

    // Simulation-like samples: the caller should provide an explicit glob.
    if (effectiveType == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE || effectiveType == ExperimentParameters::SampleType::UNIFORM_TYPE) {
        chain.Add(InputHipoFiles.c_str());
        if (PrintOut) { std::cout << env::SYSTEM_COLOR << InputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n\n" << std::flush; }
        return;
    }

    // Data samples
    if (effectiveType == ExperimentParameters::SampleType::DATA_TYPE) {
        const std::vector<std::string>* runs = nullptr;

        // Allow names that include a run suffix (e.g. "..._run_015186").
        if (bt::FindSubstring(sn, "H1_data_2GeV") || bt::FindSubstring(sn, "H1_data_2070MeV")) {
            runs = &lists::H1_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "D2_data_2GeV") || bt::FindSubstring(sn, "D2_data_2070MeV")) {
            runs = &lists::D2_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "C12_data_2GeV") || bt::FindSubstring(sn, "C12_data_2070MeV")) {
            runs = &lists::C12_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_2GeV") || bt::FindSubstring(sn, "Ar40_data_2070MeV")) {
            runs = &lists::Ar40_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "C12_data_4GeV") || bt::FindSubstring(sn, "C12_data_4029MeV")) {
            runs = &lists::C12_data_4GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_4GeV") || bt::FindSubstring(sn, "Ar40_data_4029MeV")) {
            runs = &lists::Ar40_data_4GeV_runs;
        } else if (bt::FindSubstring(sn, "H1_data_6GeV") || bt::FindSubstring(sn, "H1_data_5986MeV")) {
            runs = &lists::H1_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "D2_data_6GeV") || bt::FindSubstring(sn, "D2_data_5986MeV")) {
            runs = &lists::D2_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "C12x4_data_6GeV") || bt::FindSubstring(sn, "C12x4_data_5986MeV")) {
            runs = &lists::C12x4_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_6GeV") || bt::FindSubstring(sn, "Ar40_data_5986MeV")) {
            runs = &lists::Ar40_data_6GeV_runs;
        }

        // Match the legacy behavior exactly:
        // Expand the run list only when the user did not request a specific run directory.
        if (runs != nullptr && ReconHipoDir == "") {
            add_runs(*runs);
            return;
        }

        // Otherwise, fall back to the explicit file/glob passed by the caller.
        chain.Add(InputHipoFiles.c_str());
        return;
    }

    // Unknown/unsupported sample type: fall back to whatever the caller provided.
    chain.Add(InputHipoFiles.c_str());
}
#pragma endregion

#endif  // HIPOCHAINLOADER_H
