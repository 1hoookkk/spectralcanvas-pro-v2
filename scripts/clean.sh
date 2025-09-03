#!/usr/bin/env bash
set -euo pipefail

paths=(
  build out build_minimal Release Debug RelWithDebInfo MinSizeRel \
  x64 x86 JuceLibraryCode Builds
)

for p in "${paths[@]}"; do
  if [ -e "$p" ]; then
    echo "Removing $p"
    rm -rf "$p" || true
  fi

done

if [[ "${1:-}" == "--hard" ]]; then
  echo "Hard clean of common artifacts"
  find . -type f \( -name '*.exe' -o -name '*.dll' -o -name '*.lib' -o -name '*.pdb' -o -name '*.ilk' -o -name '*.exp' -o -name '*.obj' -o -name '*.pch' -o -name '*.idb' -o -name '*.ipdb' -o -name '*.iobj' -o -name '*.res' -o -name '*.manifest' -o -name '*.tlog' -o -name '*.lastbuildstate' \) -delete || true
fi

echo "Clean complete."

