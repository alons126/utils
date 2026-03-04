
# Acceptance maps and weight maps generator

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

This document explains what the `AMaps` class does, how it is used in `RecoAnalyzer.cpp`, and how to run it in generation vs loading modes.

## What `AMaps` does

`AMaps` builds and/or loads detector acceptance maps in `(phi, theta)` for momentum slices of:

- electrons
- protons
- neutrons

For each momentum slice, it keeps:

- truth-level hit map (`TL`)
- reconstructed hit map (`Reco`)
- ratio map (`Reco / TL`)
- binary acceptance map (`AMap`, 0/1 after thresholding the ratio)
- weight map (`WMap`, stores ratio values for accepted bins)

Core implementation is in:

- `framework/classes/AMaps/AMaps.h`
- `framework/classes/AMaps/AMaps.cpp`

## Runtime modes

`AMaps` has two main modes via `Maps_Mode`:

- `AMaps`: standard acceptance maps (binary maps used for fiducial acceptance)
- `WMaps`: weight maps (continuous map values used for acceptance correction)

Important: a single `AMaps` instance runs in one mode at a time.  
Saving and loading are mode-driven (not mixed).

## How `RecoAnalyzer` uses it

### 1. Settings phase

`AcceptanceMapsSettings` controls behavior:

- `Generate_Electron_AMaps`
- `Generate_Nucleon_AMaps`
- `Generate_WMaps`
- `AMaps_calc_with_one_reco_electron`
- bin profiles: `P_e_bin_profile`, `P_nuc_bin_profile`
- slice test toggles: `Electron_single_slice_test`, `Nucleon_single_slice_test`, `TestSlices`

Important defaults/guards:

- both generation flags start `false`
- data runs force map generation off
- electron and nucleon generation flags cannot both be `true` at once

See:

- `framework/structures/RecoAnalyzerSettings/AcceptanceMapsSettings.h`

### 2. Constructing `AMaps`

In `RecoAnalyzer.cpp`:

- If generating maps: use generation constructor with sample name, bin profiles, beam energy, output path, and bin counts.
- If not generating: use loading constructor with acceptance maps directory and beam energy.

This happens around:

- `applications/RecoAnalyzer.cpp` lines ~670-678

### 3. Event loop usage

`RecoAnalyzer` uses `AMaps` in two ways:

- `hFillMaps("TL"/"Reco", particle, p, theta, phi, weight)` to build map histograms during generation
- `IsInFDQuery(...)` to apply fiducial acceptance logic (either from map matching or direct theta cut, depending on generation/load state)

Typical calls:

- truth particles: lines ~10917, ~10935, ~10964 (query), ~12005/~12027/~12112 (fill)
- reco particles: lines ~13396/~13417/~13462 (fill)
- reco analysis cuts: lines ~12397, ~12410, ~12432 (query)

## Generation pipeline inside `AMaps`

Called by:

- `aMaps_master.DrawAndSaveMaps(...)` in `RecoAnalyzer.cpp` (line ~24547)

`DrawAndSaveMaps` does:

1. `CalcAcceptanceEfficiency()`: compute `Reco/TL` per slice.
2. `GenerateFilteredRecoMaps(...)`: zero bins below threshold.
3. `GenerateMapMatrices(...)`: build in-memory `AMap` (0/1) and `WMap` matrices.
4. `GenerateExtendedElectronMapMatrices(...)`: extends electron acceptance slices.
5. `SaveMaps(...)`: write mode-specific `.par` slice files and limits.
6. Draw/save per-slice and combined maps to PDF/ROOT.

## Loading pipeline inside `AMaps`

Loading constructor:

- infers source folders from beam energy:
  - `Uniform_1e_sample_<E>`
  - `Uniform_ep_sample_<E>`
  - `Uniform_en_sample_<E>`
- reads slice limits files (`*_slice_limits.par`)
- reads mode-specific map slices (`.par`)
- sets histogram grid sizes from loaded slices (with defaults if unavailable)

Map matching then uses:

- `MatchAngToHitMap(...)`: find momentum slice + angular bin and return accepted/not accepted
- `IsInFDQuery(...)`: combines map acceptance with theta FD cut

### Unified loading functions

Loading now uses unified helpers:

- `ReadMapSlices(...)`:
  - dispatches by `Maps_Mode`
  - reads `std::vector<std::vector<std::vector<int>>>` in `AMaps` mode
  - reads `std::vector<std::vector<std::vector<double>>>` in `WMaps` mode
- `ReadMap(...)`:
  - parses `Line` rows in `AMaps` mode
  - parses `Weight` rows in `WMaps` mode
  - prints warnings through `bt::PrintWarning(...)` when files are missing

## Output layout

When generating, `AMaps` writes under the chosen acceptance maps directory according to the active mode:

- `AMaps` mode:
  - `<SampleName>/e_AMaps_by_slice/`
  - `<SampleName>/p_AMaps_by_slice/`
  - `<SampleName>/n_AMaps_by_slice/`
  - `<SampleName>/n_AMaps_by_ConstSlice/`
  - `<SampleName>/e_extended_AMaps_by_slice/` (electron extension)
- `WMaps` mode:
  - `<SampleName>/e_WMaps_by_slice/`
  - `<SampleName>/p_WMaps_by_slice/`
  - `<SampleName>/n_WMaps_by_slice/`
  - `<SampleName>/n_WMaps_by_ConstSlice/`

Backward compatibility:

- WMap loading supports legacy folders/files named with singular `WMap` (for previously generated outputs).

Also produced:

- plus ROOT bundles with prefixes like:
  - `00_AMaps_BC_-_`
  - `01_AMap_TL_-_`
  - `02_AMap_Reco_-_`
  - `03_AMap_Ratio_-_`
  - `04_cPart_Sep_AMaps_-_`
  - `05_AMaps_-_`

Each slice `.par` file stores:

- lower/upper momentum limits
- grid dimensions
- matrix rows with `:`-separated values:
  - `Line` rows in `AMaps` mode
  - `Weight` rows in `WMaps` mode

## Binning profiles

Supported electron profiles:

- `reformat_e_bins`
- `varying_P_e_bins`
- `uniform_P_e_bins`
- `equi_inverted_P_e`

Supported nucleon profiles:

- `equi_inverted_P_nuc`
- `varying_P_nuc_bins`
- `uniform_P_nuc_bins`

Configured in `AcceptanceMapsSettings` and consumed by `AMaps::SetElectronBins` / `AMaps::SetBins`.

## Practical usage patterns

### Generate new acceptance maps

1. Set one of:
   - `Generate_Electron_AMaps = true` (uniform electron sample)
   - `Generate_Nucleon_AMaps = true` (uniform ep/en sample)
2. Run `RecoAnalyzer`.
3. At the end, `DrawAndSaveMaps` writes map products to disk.

### Use existing acceptance maps during analysis

1. Keep generation flags false.
2. Ensure `AcceptanceMapsDirectory` contains the expected map folders/files.
3. `RecoAnalyzer` loads maps via `AMaps` loading constructor.
4. Fiducial acceptance checks go through `IsInFDQuery` + `MatchAngToHitMap`.

## Notes and caveats

- `AMaps_calc_with_one_reco_electron` gates some proton/neutron map filling logic.
- Neutron logic includes both regular slices and "const" slices.
- `IsInFDQuery` behavior changes by context:
  - generating maps: does not require map match
  - loading maps: requires map match + theta cut
