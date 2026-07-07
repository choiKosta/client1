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

echo "Install: VSCode and recommended extensions"
if command -v code >/dev/null 2>&1; then
  echo "code CLI found"
else
  run bash -lc "wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg && sudo install -o root -g root -m 644 packages.microsoft.gpg /usr/share/keyrings/ && sudo sh -c 'echo \"deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main\" > /etc/apt/sources.list.d/vscode.list' && sudo apt update && sudo apt install -y code"
fi

EXTS=(ms-vscode.cpptools ms-vscode.cmake-tools twxs.cmake ms-vscode-remote.remote-wsl ms-vscode-remote.remote-containers)
for e in "${EXTS[@]}"; do
  run code --install-extension "$e" || true
done
