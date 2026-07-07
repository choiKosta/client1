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

echo "Install: vcpkg"
if [ -d "$HOME/vcpkg" ]; then
  echo "vcpkg already exists at $HOME/vcpkg"
else
  run git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
  run bash -lc "cd $HOME/vcpkg && ./bootstrap-vcpkg.sh"
  run bash -lc "echo 'export VCPKG_ROOT=\"$HOME/vcpkg\"' >> ~/.profile"
fi
