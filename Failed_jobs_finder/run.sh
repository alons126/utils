#!/bin/bash

set -euo pipefail

# Absolute path to this script's directory (so we can run Failed_jobs_finder.py reliably)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colors (override by exporting COLOR_START / COLOR_END before running)
: "${COLOR_START:=$'\033[35m'}"
: "${COLOR_END:=$'\033[0m'}"

banner() {
  echo ""
  echo "${COLOR_START}=======================================================================${COLOR_END}"
  printf "%s%s%s\n" "${COLOR_START}=" "$1" "=${COLOR_END}"
  echo "${COLOR_START}=======================================================================${COLOR_END}"
  echo ""
}

info() {
  echo "${COLOR_START}$1${COLOR_END}"
}

# ----------------------------
# Configuration
# ----------------------------
# If PyDir is already exported in your environment, we keep it.
# Otherwise, we default to your production directory.
: "${PyDir:=/pnfs/genie/scratch/users/asportes/2N_Analysis_Samples/Ar40/GEM21_11a_00_000/2070MeV_Q2_0_02}"

# Name of the directory listing file (stored under PyDir)
: "${List_File:=list.txt}"

export PyDir
export List_File

# Validate PyDir
if [[ ! -d "$PyDir" ]]; then
  echo "Error: PyDir is not a directory: $PyDir"
  exit 1
fi

banner "Running Failed_jobs_finder"
info "Using PyDir: $PyDir"
info "Using List_File: $List_File"

# ----------------------------
# Regenerate List_File
# ----------------------------
banner "Regenerating $List_File"

src_dir="$PyDir/master-routine_validation_01-eScattering"
if [[ ! -d "$src_dir" ]]; then
  echo "Error: directory not found: $src_dir"
  exit 1
fi

list_path="$PyDir/$List_File"
rm -f "$list_path"

# One entry per line
ls -1 "$src_dir" > "$list_path"
info "Wrote directory listing to: $list_path"

# ----------------------------
# Duplicate grid_submission.xml with run index
# ----------------------------
banner "Duplicating grid_submission.xml"

# Find the first grid_submission.xml anywhere under PyDir
src_xml="$(find "$PyDir" -type f -name "grid_submission.xml" -print -quit)"
if [[ -z "$src_xml" ]]; then
  echo "Error: grid_submission.xml not found anywhere under: $PyDir"
  exit 1
fi

src_xml_dir="$(cd "$(dirname "$src_xml")" && pwd)"

i=1
while [[ -f "$src_xml_dir/grid_submission_run_${i}.xml" ]]; do
  ((i++))
done

dst_xml="$src_xml_dir/grid_submission_run_${i}.xml"
cp "$src_xml" "$dst_xml"

echo "Copied:"
echo "  $src_xml"
echo "  -> $dst_xml"

# ----------------------------
# Launch
# ----------------------------
banner "Launching"
python3 "$SCRIPT_DIR/Failed_jobs_finder.py"

banner "Operation finished"