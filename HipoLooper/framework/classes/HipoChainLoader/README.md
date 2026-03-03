# The `HipoChainLoader` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`HipoChainLoader` is a small utility that builds a `clas12root::HipoChain` from one or more file globs while safely filtering corrupted HIPO files.

The key problem it solves is that some corrupted HIPO files can trigger `abort()` or assertions inside the reader stack. Those failures cannot be handled with C++ exceptions. HipoChainLoader avoids crashing your analysis by probing each file in a separate child process using `fork()`.

## Main idea

1. Expand a glob pattern (or multiple patterns) into an explicit list of files.
2. For each file, fork a child process.
3. In the child process, attempt to open the file with `hipo::reader` and optionally verify `nRecords > 0`.
4. Only files that pass the probe are added to the chain.

## Public API

### `Build`

Build into an existing chain from a single glob pattern.

- Input: `glob_pattern` such as `/path/to/reconhipo/*.hipo`
- Output: `Result` summary with lists of added and skipped files

### `BuildPtr`

Convenience wrapper that allocates the chain and returns `{chainPtr, result}`.

### `BuildFromList`

Build into an existing chain using `ExperimentParameters`.

This reproduces the legacy behavior of run-list expansion for data samples:

- If the sample is data and `ReconHipoDir` is empty, it expands into per-run globs using the predefined run lists in `lists.h`.
- Otherwise it uses the experiment-provided input glob (`Experiment.GetInputFiles()`).

### `BuildPtrFromList`

Convenience wrapper that allocates the chain and builds it from `ExperimentParameters`.

## Options

`HipoChainLoader::Options` controls logging, printing, reader tags, and the probe condition.

Common settings:

- `log_skipped`: whether to log skipped files to disk
- `log_path`: where to write the skipped-files log
- `append_log`: append or overwrite the log file
- `reader_tags`: tags passed to `clas12root::HipoChain::SetReaderTags` (must be `std::vector<long>`)
- `turn_off_qadb`: call `chain.db()->turnOffQADB()`
- `require_positive_records`: if true, reject files with `nRecords <= 0`

## Result

`HipoChainLoader::Result` provides:

- `n_globbed`: number of files found by glob expansion
- `n_added`: number of files added to the chain
- `n_skipped`: number of files rejected
- `added_files`: list of accepted paths
- `skipped_files`: list of rejected paths

The class also maintains per-build counters that can be accessed via:

- `GetNumOfGoodHipoFiles()`
- `GetNumOfBadHipoFiles()`
- `GetTotalNumOfHipoFiles()`

## Example usage

### Build from a single glob

```cpp
HipoChainLoader::Options opt{};
opt.log_skipped = true;
opt.log_path = OutputDir + pd::skipped_files_list_prefix;
opt.reader_tags = {0L};

HipoChainLoader loader(opt);

auto [chainPtr, res] = loader.BuildPtr("/path/to/reconhipo/*.hipo", "MySample");
auto& chain = *chainPtr;
```

### Build using ExperimentParameters

```cpp
ExperimentParameters exp(RecoSamplePath, ReconHipoDir);

HipoChainLoader loader;
auto [chainPtr, res] = loader.BuildPtrFromList(exp);
auto& chain = *chainPtr;
```

## Notes and caveats

- Probing uses `fork()`, so it is intended for Linux and other POSIX systems.
- If you run on many files, the fork-probe is safer but slower than blindly adding all files.
- The loader throws if no files are found or if all files are rejected.
