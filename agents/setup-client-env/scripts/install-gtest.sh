#!/usr/bin/env bash
set -euo pipefail

DRY_RUN=true
for arg in "$@"; do
  case "$arg" in
    --run) DRY_RUN=false; shift;;
    --dry-run) DRY_RUN=true; shift;;
    -h|--help) echo "Usage: $0 [--dry-run|--run]"; exit 0;;
  esac
done

run(){
  if [ "$DRY_RUN" = true ]; then
    echo "[DRY-RUN] $*"
  else
    echo "+ $*"
    eval "$@"
  fi
}

echo "Install: Google Test via vcpkg (recommended) or apt (simple)"
if [ -d "$HOME/vcpkg" ]; then
  run bash -lc "$HOME/vcpkg/vcpkg install gtest"
else
  run sudo apt install -y libgtest-dev
  echo "Note: libgtest-dev installs sources; you must build and install gtest yourself or use vcpkg."
fi
