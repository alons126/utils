
# General utilities

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

This directory contains small, mostly header-only namespaces that are shared across the analysis framework. The goal is to keep these utilities broadly reusable and avoid circular dependencies between higher-level classes.

A common pattern in this repository is to keep functionality grouped by *purpose* into namespaces (rather than monolithic classes), and then expose a convenient “umbrella include” via `utilities.h`.

## How this folder is meant to be used

- Include **individual headers** when you only need a specific utility.
- Include **`utilities.h`** when you want the full analysis utility stack (ROOT + YAML loader + math/physics helpers + histogram helpers).

Because some components are implemented as header-only (including a few `.cpp` files that are intentionally included as implementation headers), be careful to include them **exactly once per translation unit** and avoid mixing different include orders across compilation units.

---

## Overview of provided namespaces

### `basic_tools` (`basic_tools.h`)

Lowest-level helper functions that are safe to use everywhere.

Key functionality:

- **Environment / runtime helpers**
  - `CheckSSHConnection()` and `CheckSSHConnectionAndHost()` print whether the process runs under SSH and, if possible, resolve the remote host.
  - `GetSSHHostName()` returns the resolved remote host name (empty string on failure).
  - `GetCurrentDirectory()` returns the current working directory.

- **Small conversions**
  - `BoolToChar(bool)` returns `"true"` or `"false"`.
  - `BoolToString(bool)` returns a `std::string` version.
  - `ToStringWithPrecision(value, n)` formats numbers with fixed precision.

- **String utilities**
  - `FindSubstring(haystack, needle)` does a simple substring search.
  - `ReplaceSubstring(input, toReplace, replaceWith)` replaces the first occurrence.
  - `trim(str)` removes leading and trailing whitespace.

Design notes:

- `basic_tools` is intended to have **no dependencies** on other analysis namespaces or framework classes.
- It depends only on the C++ standard library and `environment.h` (used for consistent colored output).

---

### `constants` (`constants.h`)

Central place for physical constants and a few CLAS12-specific defaults.

Includes:

- Particle masses in GeV: `m_n`, `m_p`, `m_e`, `m_pizero`, etc.
- Speed of light: `c` in cm/ns.
- PDG IDs for common particles.
- CLAS12 theta ranges implemented as `DSCuts` objects:
  - `ThetaFD` (FD range)
  - `ThetaCD` (CD range)

---

### `Yaml_loader` (`Yaml_loader.h`, `Yaml_loader.cpp`)

Loads a `DataSample` struct from a YAML configuration file.

Entry point:

- `DataSample LoadFromYamlFile(const std::string& yaml_path);`

Behavior:

- Parses YAML using `yaml-cpp`.
- Accepts multiple key spellings for backward compatibility (for example `SampleFilesTopDir` or `sample_file_path`).
- Performs basic validation after loading:
  - `IsSimulationSample` and `IsDataSample` cannot both be true or both be false.
  - Only one of `BeamAt2GeV`, `BeamAt4GeV`, `BeamAt6GeV` may be true.

Notes and conventions:

- `Yaml_loader.cpp` is used as an **implementation header** (it is included from `utilities.h`).
  - This makes the loader effectively header-only in the current structure.
  - If you later convert this to a normal compiled `.cpp`, update includes accordingly.

Minimal example:

```cpp
#include "framework/namespaces/general_utilities/Yaml_loader/Yaml_loader.h"

auto sample = Yaml_loader::LoadFromYamlFile("config/sample.yaml");
std::cout << sample.SampleName << std::endl;
```

---

### `data_processor` (`data_processor.h`)

String-parsing helpers for labeling plots and deriving human-readable names.

Highlights:

- `GetFS(Source)` derives a final-state label from a string by matching known patterns.
- `GetParticleName(Source, PluralParticles)` returns `Electron`, `Proton`, `Neutron`, `Photon`, etc. based on pattern matching.

These functions are used heavily for:

- histogram naming conventions
- automatic titles and annotations

---

### `lists` (`lists.h`)

Central map of variable metadata used for labeling histograms.

- `VariableNames` maps an internal variable key to:
  - `VarName`  (canonical name)
  - `VarLabel` (ROOT TLatex label)
  - `VarDim`   (units string)

This enables consistent axis labeling across the project.

---

### `analysis_physics` (`analysis_physics.h`)

Kinematic helpers for reconstructed or truth-level analysis.

Includes functions such as:

- `CalcQ2(Beam_4v, P_e_4v)`
- `CalcxB(Beam_4v, P_e_4v, omega, Nuc_mass)`
- `CalcW(omega, q_3v, Nuc_mass)`
- `CalcPmiss1N3v(P_nucL_3v, q_3v)`
- `CalcEmiss1N(omega, E_nucL, mass_nucL)`
- `CalcPmiss2N3v(P_nucL_3v, P_nucR_3v, q_3v)`
- `CalcEmiss2N(omega, E_nucL, E_nucR, mass_nucL)`

All functions follow the same pattern:

- Inputs are `TLorentzVector` and `TVector3` objects.
- Mass defaults use `constants::m_p` unless specified.

---

### `analysis_math` (`analysis_math.h`)

Math helpers that are used throughout reconstruction and plotting.

Provides:

- Degree/radian conversion: `RadToDeg`, `DegToRad`.
- Common angle computations: `CalcTheta_deg`, `CalcPhi_deg`, etc.
- Misc helpers like `RadCalc`.
- Experiment-specific helpers such as `GetPhi_e(OutPutFolder, phi_N)`.

Polynomial solver integration:

- `analysis_math.h` includes `poly_solver.cpp` to expose polynomial root solvers.
- The solver code lives in `analysis_math/poly_solver.h` and `analysis_math/poly_solver.cpp`.

Important note on include guards:

- `poly_solver.h` currently uses `#ifdef POLY_SOLVER_H` rather than the usual `#ifndef` pattern.
  - This means its declarations are *not* active unless `POLY_SOLVER_H` is defined before including the header.
  - In the current setup, `analysis_math.h` includes the **implementation** via `poly_solver.cpp`, which itself includes `poly_solver.h`.
  - If you later want to use `poly_solver.h` directly, fix the guard to the conventional `#ifndef POLY_SOLVER_H` form.

---

### `variable_correctors` (`variable_correctors.h`)

Helpers for extracting peak positions from histograms.

Main tools:

- `GetPeakFromHighestBin(TH1D*)`
  - Returns the bin center at the maximum bin and estimates uncertainty as half a bin width.

- `FitPeakToGaussian(TH1D*, std::vector<double> fitLimits = {})`
  - Fits a Gaussian in a chosen range.
  - If no range is provided, it infers a range based on the histogram name and peak position.
  - Returns the fitted mean and its uncertainty.

Notes:

- The fit function is created with `new TF1(...)` and attached to the histogram.
- The code clears and replaces the histogram function list, so it will overwrite previously attached fit functions.

---

### `reco_analysis_functions` (`reco_analysis_functions.h`)

Utilities that operate directly on `clas12::region_part_ptr` objects.

Examples:

- `GetFDNeutronP(Neutron, apply_nucleon_cuts)`
  - For PDG 2112 neutrons, returns `Neutron->getP()`.
  - For PDG 22 “photon-tagged neutrals”, computes a momentum proxy using beta and gamma.
  - If `apply_nucleon_cuts` is false, uses the momentum stored in the bank.

- `CheckForNeutralFDECALHits(...)`
  - Determines whether a neutral hit PCAL/ECIN/ECOUT by inspecting calorimeter bank info.

These functions encapsulate reconstruction-level conventions that are used repeatedly across analysis scripts.

---

### `histogram_functions` (`histogram_functions.h`)

Helpers for filling histograms consistently by interaction type and for filling `hsPlots` objects.

Main patterns:

- `FillByInt1D(...)` and `FillByInt2D(...)`
  - Always fill the inclusive histogram.
  - Additionally fill one of QEL/MEC/RES/DIS based on boolean flags.

- `FillByInthsPlots(...)`
  - Same logic as above but works on `hsPlots` (your sliced-histogram manager).
  - Supports both `hsPlots::TH1D_TYPE` and `hsPlots::TH2D_TYPE`.

---

### `utilities` (`utilities.h`)

Umbrella header that pulls together the common utilities used throughout the project.

It includes:

- ROOT fundamentals (TFile, TH1, TLorentzVector, etc.)
- YAML loader (`Yaml_loader/Yaml_loader.cpp`)
- math/physics helpers (`analysis_math`, `analysis_physics`)
- basic tools, constants, lists
- reco helpers and histogram helpers

It also provides additional small utility functions such as:

- `ConfigRegion(histName)` which infers FD/CD from a histogram name.
- `ConfigSampleType(sampleName)` which infers simulation vs data from the sample name.
- `GetHistogramFromVector(HistoList, key, option)` to find histograms by name or title.
- `FillHistogramByProcess(...)` which fills QEL/MEC/RES/DIS histograms.

Because `utilities.h` includes a lot, prefer it for:

- analysis applications
- macros
- plotting scripts

and prefer smaller includes for:

- library code
- unit tests
- code that should compile fast

---

## Practical tips

- If you see multiple-definition linker errors, check whether an implementation `.cpp` is being included from more than one place.
- Keep `basic_tools` dependency-free to avoid circular include chains.
- When adding new variables, update `lists::VariableNames` so labels stay consistent.
- When adding new YAML keys, add them as aliases in the loader rather than breaking old configs.