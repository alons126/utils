//
// Created by Alon Sportes on 03/03/2026.
//

#ifndef HIPOCHAINLOADER_H
#define HIPOCHAINLOADER_H

#include "HipoChainLoader.h"

HipoChainLoader::HipoChainLoader(Options opt) : opt_(std::move(opt)) {}

const HipoChainLoader::Options& HipoChainLoader::GetOptions() const { return opt_; }

HipoChainLoader::Result HipoChainLoader::Build(clas12root::HipoChain& chain, const std::string& glob_pattern, const std::string& sample_name_for_log) const {
    Result res;

    const std::vector<std::string> files = ExpandGlobFiles_(glob_pattern);
    res.n_globbed = static_cast<int>(files.size());

    if (files.empty()) {
        std::ostringstream oss;
        oss << "HipoChainLoader: no files matched glob: " << glob_pattern;
        throw std::runtime_error(oss.str());
    }

    if (opt_.print_progress) { std::cout << "\nHipoChainLoader: probing " << res.n_globbed << " file(s)\n"; }

    res.added_files.reserve(files.size());
    res.skipped_files.reserve(files.size());

    int idx = 0;
    for (const auto& f : files) {
        ++idx;

        const bool good = IsGoodHipoFile_ForkGuard_(f, opt_.require_positive_records);

        if (good) {
            chain.Add(f);
            res.added_files.push_back(f);
        } else {
            res.skipped_files.push_back(f);
        }

        if (opt_.print_progress && (idx % 200 == 0 || idx == res.n_globbed)) {
            std::cout << "  checked " << idx << "/" << res.n_globbed << " | added " << res.added_files.size() << " | skipped " << res.skipped_files.size() << "\n";
        }
    }

    res.n_added = static_cast<int>(res.added_files.size());
    res.n_skipped = static_cast<int>(res.skipped_files.size());

    chain.SetReaderTags(opt_.reader_tags);
    if (opt_.turn_off_qadb) { chain.db()->turnOffQADB(); }

    if (opt_.log_skipped && !res.skipped_files.empty()) { WriteSkippedLog_(res.skipped_files, sample_name_for_log); }

    if (opt_.print_skipped && !res.skipped_files.empty()) {
        std::cout << "\nHipoChainLoader: skipped " << res.n_skipped << " bad file(s)\n";
        for (const auto& f : res.skipped_files) { std::cout << "  skipped: " << f << "\n"; }
    }

    if (res.n_added == 0) {
        std::ostringstream oss;
        oss << "HipoChainLoader: after filtering, chain has 0 files. Glob: " << glob_pattern;
        throw std::runtime_error(oss.str());
    }

    return res;
}

std::pair<std::unique_ptr<clas12root::HipoChain>, HipoChainLoader::Result> HipoChainLoader::BuildPtr(const std::string& glob_pattern, const std::string& sample_name_for_log) const {
    auto chain = std::make_unique<clas12root::HipoChain>();
    Result res = Build(*chain, glob_pattern, sample_name_for_log);
    return {std::move(chain), std::move(res)};
}

std::vector<std::string> HipoChainLoader::ExpandGlobFiles_(const std::string& pattern) {
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

bool HipoChainLoader::IsGoodHipoFile_ForkGuard_(const std::string& file, bool require_positive_records) {
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

std::string HipoChainLoader::NowString_() {
    using clock = std::chrono::system_clock;
    const auto now = clock::now();
    const std::time_t t = clock::to_time_t(now);

    std::tm tm_buf;
    localtime_r(&t, &tm_buf);

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void HipoChainLoader::WriteSkippedLog_(const std::vector<std::string>& skipped, const std::string& sample_name_for_log) const {
    std::ios_base::openmode mode = std::ios::out;
    mode |= (opt_.append_log ? std::ios::app : std::ios::trunc);

    std::ofstream out(opt_.log_path, mode);
    if (!out) {
        std::cerr << "\nHipoChainLoader: could not open log file: " << opt_.log_path << "\n";
        return;
    }

    out << "============================================================\n";
    out << "Timestamp: " << NowString_() << "\n";
    if (!sample_name_for_log.empty()) { out << "Sample: " << sample_name_for_log << "\n"; }
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

                    if (PrintOut) { std::cout << TempInputHipoFiles << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { std::cout << "\n"; }
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

                    if (PrintOut) { std::cout << TempInputHipoFiles << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { std::cout << "\n"; }
            }
        } else {
            chain.Add(InputHipoFiles.c_str());
        }
    } else if (Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE) {
        chain.Add(InputHipoFiles.c_str());

        if (PrintOut) { std::cout << InputHipoFiles << " directory added to HipoChain!\n\n"; }
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
            if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!\n" << env::RESET_COLOR; }
        }
        if (PrintOut) { std::cout << env::SYSTEM_COLOR << "\n" << env::RESET_COLOR; }
    };

    // Simulation-like samples: the caller should provide an explicit glob.
    if (effectiveType == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE || effectiveType == ExperimentParameters::SampleType::UNIFORM_TYPE) {
        chain.Add(InputHipoFiles.c_str());
        if (PrintOut) { std::cout << env::SYSTEM_COLOR << InputHipoFiles << " directory added to HipoChain!\n\n" << env::RESET_COLOR; }
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
