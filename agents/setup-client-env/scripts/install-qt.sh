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

echo "Install: Qt development packages"
run sudo apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
