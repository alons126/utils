# The `Directories` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`Directories` is a small utility class that centralizes the creation and naming of the analysis output folder structure.

It is responsible for:
- Creating a root plots folder (default: `./plots`).
- Creating a consistent hierarchy of subfolders for different analysis stages and plot categories.
- Publishing canonical directory paths through a set of `std::map<std::string, std::string>` members, so that the rest of the analysis code writes outputs into predictable locations.

The class is designed to be constructed once near the beginning of an analysis job, and then passed around (or referenced globally) wherever plot output paths are needed.


## Class description (Doxygen)

/**
 * @class Directories
 *
 * Central directory configuration object used to create and expose the analysis output folder structure.
 *
 * Purpose:
 *   Create (and optionally clear) a hierarchy of plot-output directories under a user-provided root folder,
 *   and publish canonical absolute paths via maps (e.g. `Nphe_dir_map`, `Chi2_dir_map`, etc.) so that all
 *   analysis modules write outputs into consistent locations.
 *
 * Notes:
 *   - The constructor is the mutator: it creates directories and fills the directory maps.
 *   - `plots_path` is treated as the root plots folder.
 *   - When `Clear_Old_Directories` is true, the constructor clears the root plots folder contents.
 */


## Files

- `Directories.h`
  - Declares the `Directories` class.
  - Defines the folder-name constants and the list of daughter folders for each plot category.
  - Declares the output maps.

- `Directories.cpp`
  - Implements the constructor that builds the directory tree and fills all maps.
  - Implements helper methods (`FindSubstring`, `Find`) and `MakeDirectory`.


## How it works

### 1) Root plots folder

The constructor takes:
- `plots_path`: the root folder where all plot output is written.
- `Clear_Old_Directories`: whether to remove existing contents under the root plots folder before creating the new structure.

Behavior:
- Always ensures the root plots folder exists (`mkdir -p`).
- If `Clear_Old_Directories == true`, clears the root folder content (`rm -r <root>/*`).

Important:
- Clearing is recursive. If you pass a non-dedicated folder as `plots_path`, you can delete unrelated files.


### 2) Directory trees per plot category

Each plot category is implemented with:
- A boolean flag `create_*_Dir` that enables or disables the directory creation.
- A `*_Parent_Directory` string (relative path under the root folder).
- A `*_Daughter_Folders` `std::vector<std::string>` that enumerates subfolders.

The constructor loops over the daughter list and calls `MakeDirectory(...)` to create each folder.


### 3) Canonical path maps

After creating each directory tree, the constructor fills a corresponding map with canonical directory paths.

Examples:
- `Nphe_dir_map["Nphe_Parent_Directory"]`
- `Chi2_dir_map["Chi2_1e_cut_Directory"]`
- `Vertex_dir_map["Vertex_dV_1p_Directory"]`

These strings typically end with a trailing slash `/`.


## Public API

### Constructor

```cpp
Directories(const std::string& plots_path = "./plots", const bool Clear_Old_Directories = true);
```

Typical usage:
```cpp
Directories dirs("./plots", true);

// Example: save Nphe plots into the 1p directory.
const std::string out_dir = dirs.Nphe_dir_map.at("Nphe_1p_Directory");
```


### `MakeDirectory`

```cpp
void MakeDirectory(const bool& Create_Directory, const std::string& Plots_Parent_Folder, const std::string& Plots_Daughter_Folder, const bool& Clear_Parent_Folder_content = false, const std::string& Parent_Folder = "./plots");
```

Semantics:
- Creates `Parent_Folder/Plots_Parent_Folder/Plots_Daughter_Folder` if `Create_Directory` is true.
- Optionally clears the content of the created directory when `Clear_Parent_Folder_content` is true.


### Helper functions

- `FindSubstring(string string1, std::string string2)`
  - Utility that checks whether `string2` appears in `string1`.

- `Find(std::vector<std::string> vector, std::string string)`
  - Searches the provided vector for a matching entry and returns it.
  - Used to keep the directory-map keys consistent with the daughter-folder vectors.


## Directory maps

The class exposes one map per plot category. Each map contains keys that are used throughout the analysis to choose the correct output folder.

Common categories:
- Cut monitoring
  - `Nphe_dir_map`
  - `Chi2_dir_map`
  - `Vertex_dir_map`
  - `SF_dir_map`
  - `ECAL_fiducial_dir_map`
  - `Momentum_dir_map`

- Physics and monitoring
  - `ReacMon_dir_map`
  - `W_dir_map`
  - `Beta_dir_map`
  - `Angle_dir_map`
  - `Momentum_transfer_dir_map`
  - `E_e_dir_map`
  - `Omega_dir_map`
  - `Ecal_dir_map`
  - `TKI_dir_map`
  - `ToF_dir_map`

- Efficiency, acceptance, and validation
  - `Eff_and_ACorr_dir_map`
  - `AMaps_dir_map`
  - `Resolution_dir_map`
  - `Multiplicity_dir_map`


## Conventions and expectations

- Directory paths stored in maps usually end with `/`.
- Parent and daughter folder names are treated as relative paths under `plots_path`.
- Most folder lists include an empty string `""` as the first entry. This creates the parent directory itself.
- This class uses shell commands (`system`) for `mkdir` and `rm`.


## Safety notes

- Do not pass a sensitive folder as `plots_path` when `Clear_Old_Directories` is true.
- If your plot path can contain spaces, ensure the implementation quotes paths when invoking `system()`.


## Extending the directory structure

To add a new plot category:
1. In `Directories.h`:
   - Add `create_<New>_Dir`.
   - Add `<New>_Parent_Directory`.
   - Add `<New>_Daughter_Folders`.
   - Add `<New>_dir_map` as a public `std::map<std::string, std::string>`.

2. In `Directories.cpp` constructor:
   - Loop over `<New>_Daughter_Folders` and call `MakeDirectory(...)`.
   - Fill `<New>_dir_map` with keys that the rest of the analysis will use.

3. Update analysis code to use `<New>_dir_map.at("...")` keys.


## Troubleshooting

- If a directory key is missing, check:
  - That the corresponding daughter folder string exists in the relevant `*_Daughter_Folders` vector.
  - That the constructor filled the map entry.

- If directories are not created:
  - Check the relevant `create_*_Dir` flag.
  - Check permissions for the chosen `plots_path`.

- If you see compile errors involving `Find(...)` or stray statements:
  - Ensure any disabled assignments are fully commented out.
