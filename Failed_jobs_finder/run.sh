#!/bin/bash

unsetenv CODE_PATH
setenv CODE_PATH `pwd`

# Check and set COLOR_START and COLOR_END if not already defined
if (! $?COLOR_START) then
    unset COLOR_START
    setenv COLOR_START '\033[35m'
endif

if (! $?COLOR_END) then
    unset COLOR_END
    setenv COLOR_END '\033[0m'
endif

echo ""
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Running Failed_jobs_finder                                          =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

export PyDir=/pnfs/genie/scratch/users/asportes/2N_Analysis_Samples/Ar40/GEM21_11a_00_000/2070MeV_Q2_0_02
echo "${COLOR_START}Using PyDir: $PyDir${COLOR_END}"
echo ""

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Update review                                                       =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

# Display the latest commit in the current branch. The -1 option limits the output to one commit, and the
# --oneline option formats the output to show only the commit hash and the commit message in a single line.
# This command is useful for quickly checking the latest commit in the current branch without displaying
# the full commit history.

echo "${COLOR_START}HEAD:${COLOR_END}"
git log -1 --oneline
echo ""

echo "${COLOR_START}- Cleaning excessive files --------------------------------------------${COLOR_END}"
echo ""

# Clean the working tree by recursively removing files that are not under version control, starting from
# the current directory. The -f option is used to force the removal of files, and the -d option is used
# to remove untracked directories. The -x option is used to remove files that are ignored by git.

# This command is useful for cleaning up the working tree and removing any untracked files or directories
# that may have been created during development, like generated cut files and acceptance and weight files.

git clean -fxd # removes untracked files and directories
echo ""

echo "${COLOR_START}- Pulling repository --------------------------------------------------${COLOR_END}"
echo ""

# This command is used to reset the current branch to the latest commit in the remote repository. The
# --hard option is used to discard any local changes, and the git pull command is used to fetch and merge
# the latest changes from the remote repository.

git reset --hard
git pull
echo ""

echo "${COLOR_START}- Regenerating $List_File ---------------------------------------------${COLOR_END}"
echo ""

export List_File="list.txt"

# Check text file
if [[ -f "$PyDir/$List_File" ]]; then
    #  echo "Removing existing $List_File in $PyDir"
    rm "$PyDir/$List_File"
fi

ls  $PyDir/"master-routine_validation_01-eScattering >" $PyDir/$List_File

echo "${COLOR_START}- Duplicating grid_submission.xml -------------------------------------${COLOR_END}"
echo ""

# Fail fast
set -euo pipefail

# Check PyDir
if [[ -z "${PyDir:-}" ]]; then
    echo "Error: PyDir is not set"
    exit 1
fi

src="$PyDir/grid_submission.xml"

# Check source file
if [[ ! -f "$src" ]]; then
    echo "Error: grid_submission.xml not found in $PyDir"
    exit 1
fi

# Find next available run number
i=1
while [[ -f "$PyDir/grid_submission_run_${i}.xml" ]]; do
  ((i++))
done

dst="$PyDir/grid_submission_run_${i}.xml"

# Copy grid_submission.xml file to new file:
cp "$src" "$dst"

echo "Copied:"
echo "  $src"
echo "→ $dst"

echo "${COLOR_START}- Lunching ------------------------------------------------------------${COLOR_END}"
echo ""
python Failed_jobs_finder.py
echo ""
echo "${COLOR_START}- Operation finished --------------------------------------------------${COLOR_END}"
echo ""

#!/bin/bash

set -euo pipefail

# Absolute path to this script's directory (so we can run Failed_jobs_finder.py reliably)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CODE_PATH="$SCRIPT_DIR"

# Colors (can be overridden by exporting COLOR_START / COLOR_END before running)
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
# User inputs / environment
# ----------------------------
# PyDir must point to the production directory
: "${PyDir:?Error: PyDir is not set. Example: export PyDir=/path/to/production}" 

# Where Failed_jobs_finder.py expects the directory listing file
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
# (Optional) Repo hygiene
# ----------------------------
banner "Update review"
if git -C "$SCRIPT_DIR" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  info "HEAD:"
  git -C "$SCRIPT_DIR" log -1 --oneline
  echo ""

  info "- Cleaning excessive files --------------------------------------------"
  git -C "$SCRIPT_DIR" clean -fxd
  echo ""

  info "- Pulling repository --------------------------------------------------"
  git -C "$SCRIPT_DIR" reset --hard
  git -C "$SCRIPT_DIR" pull
  echo ""
else
  info "Not a git repo: $SCRIPT_DIR (skipping clean/pull)"
  echo ""
fi

# ----------------------------
# Regenerate List_File
# ----------------------------
banner "Regenerating $List_File"

list_path="$PyDir/$List_File"
if [[ -f "$list_path" ]]; then
  rm -f "$list_path"
fi

# You intended: list the contents of master-routine_validation_01-eScattering
src_dir="$PyDir/master-routine_validation_01-eScattering"
if [[ ! -d "$src_dir" ]]; then
  echo "Error: directory not found: $src_dir"
  exit 1
fi

# One entry per line, saved inside PyDir
ls -1 "$src_dir" > "$list_path"
info "Wrote directory listing to: $list_path"

# ----------------------------
# Duplicate grid_submission.xml with run index
# ----------------------------
banner "Duplicating grid_submission.xml"

# Search for the first grid_submission.xml under PyDir
src_xml="$(find "$PyDir" -type f -name "grid_submission.xml" -print -quit)"
if [[ -z "$src_xml" ]]; then
  echo "Error: grid_submission.xml not found anywhere under: $PyDir"
  exit 1
fi

src_xml_dir="$(cd "$(dirname "$src_xml")" && pwd)"

# Find next available run number in the same directory as the source xml
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