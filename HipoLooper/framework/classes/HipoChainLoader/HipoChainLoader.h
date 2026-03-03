//
// Created by Alon Sportes on 03/03/2026.
//

// #ifndef HIPOCHAINLOADER_H
// #define HIPOCHAINLOADER_H
#ifdef HIPOCHAINLOADER_H

    #include <glob.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>

    #include <algorithm>
    #include <chrono>
    #include <cstring>
    #include <fstream>
    #include <iomanip>
    #include <iostream>
    #include <memory>
    #include <sstream>
    #include <stdexcept>
    #include <string>
    #include <utility>
    #include <vector>

    // Include libraries:
    #include "../../namespaces/general_utilities/lists.h"
    #include "../../namespaces/general_utilities/utilities.h"
    #include "../../namespaces/setup/debugging.h"
    #include "../../namespaces/setup/path_definitions.h"

    // Include classes:
    #include "../../classes/ExperimentParameters/ExperimentParameters.cpp"

    // Include CLAS12 libraries:
    #include "../../includes/clas12_include.h"

namespace env = environment;
namespace pd = path_definitions;

/**
 * @class HipoChainLoader
 *
 * Build a clas12root::HipoChain from a glob pattern, while safely excluding bad HIPO files.
 *
 * Rationale:
 *   Some corrupted HIPO files can hard-abort (assert/abort/segfault) inside the reader stack.
 *   C++ try/catch cannot catch abort/assert. This loader probes files in a forked child process
 *   so bad files cannot crash the parent process.
 */
class HipoChainLoader {
   public:
    struct Options {
        bool print_progress;
        bool print_skipped;

        bool log_skipped;
        std::string log_path;
        bool append_log;

        std::vector<long> reader_tags;
        bool turn_off_qadb;

        // If true, treat files with nRecords <= 0 as bad.
        bool require_positive_records;

        Options()
            : print_progress(true),
              print_skipped(true),
              log_skipped(true),
              log_path(pd::skipped_files_list_prefix),
              append_log(true),
              reader_tags({0L}),
              turn_off_qadb(true),
              require_positive_records(true) {}
    };

    struct Result {
        int n_globbed;
        int n_added;
        int n_skipped;

        std::vector<std::string> added_files;
        std::vector<std::string> skipped_files;

        Result() : n_globbed(0), n_added(0), n_skipped(0) {}
    };

    const Options& GetOptions() const;

    explicit HipoChainLoader(Options opt = Options());

    // Build function ---------------------------------------------------------------------------------------------------------------------------------------------------

    Result Build(clas12root::HipoChain& chain, const std::string& glob_pattern, const std::string& SampleName = "") const;

    // Convenience overload that constructs and returns a heap-allocated chain.
    std::pair<std::unique_ptr<clas12root::HipoChain>, Result> BuildPtr(const std::string& glob_pattern, const std::string& SampleName = "") const;

    // Build a chain using the legacy run-list expansion logic, but still applying the fork-guard file validation.
    Result BuildFromList(clas12root::HipoChain& chain, const ExperimentParameters& Experiment) const;

    // Convenience overload that constructs and returns a heap-allocated chain.
    std::pair<std::unique_ptr<clas12root::HipoChain>, Result> BuildPtrFromList(const ExperimentParameters& Experiment) const;

    // AddToHipoChain function ------------------------------------------------------------------------------------------------------------------------------------------

    // This is the old function used to add runs to the HipoChain
    void AddToHipoChain(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                        const std::string& InputHipoFiles);

    // AddToHipoChainFromList function ----------------------------------------------------------------------------------------------------------------------------------

    void AddToHipoChainFromList(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                const std::string& InputHipoFiles);

    // Getters for file counts ------------------------------------------------------------------------------------------------------------------------------------------
    
    int GetNumOfGoodHipoFiles() const { return Num_of_good_hipo_files; }
    int GetNumOfBadHipoFiles() const { return Num_of_bad_hipo_files; }
    int GetTotalNumOfHipoFiles() const { return Total_num_of_hipo_files; }

   private:
    Options opt_;

    int Num_of_good_hipo_files = 0;
    int Num_of_bad_hipo_files = 0;
    int Total_num_of_hipo_files = 0;

    // ExpandGlobFiles function -----------------------------------------------------------------------------------------------------------------------------------------

    static std::vector<std::string> ExpandGlobFiles(const std::string& pattern);

    // IsGoodHipoFile_ForkGuard function --------------------------------------------------------------------------------------------------------------------------------

    static bool IsGoodHipoFile_ForkGuard(const std::string& file, bool require_positive_records);

    // NowString function -----------------------------------------------------------------------------------------------------------------------------------------------

    static std::string NowString();

    // WriteSkippedLog function -----------------------------------------------------------------------------------------------------------------------------------------

    void WriteSkippedLog(const std::vector<std::string>& skipped, const std::string& SampleName) const;

    // Internal: shared implementation that takes an already-expanded file list.
    Result BuildFromFiles(clas12root::HipoChain& chain, const std::vector<std::string>& files, const std::string& SampleName) const;

    // Internal: reproduce AddToHipoChainFromList decision logic, but return input glob patterns instead of mutating the chain.
    static std::vector<std::string> MakeInputGlobsFromList(const ExperimentParameters& Experiment);
};

#endif  // HIPOCHAINLOADER_H
