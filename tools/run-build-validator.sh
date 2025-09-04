#!/usr/bin/env bash
set -euo pipefail

# Simple wrapper script for build-validator agent
# Usage: ./tools/run-build-validator.sh [build_dir] [config] [plugin_name]

BUILD_DIR="${1:-build}"
CONFIG="${2:-RelWithDebInfo}"
PLUGIN="${3:-SpectralCanvasPro}"

echo "[build-validator] Running with BUILD_DIR=${BUILD_DIR}, CONFIG=${CONFIG}, PLUGIN=${PLUGIN}"

# Use Claude Code's Task tool to run the build-validator agent
claude -p "Run build-validator with build_dir=${BUILD_DIR}, config=${CONFIG}, plugin_name=${PLUGIN}"

exit $?