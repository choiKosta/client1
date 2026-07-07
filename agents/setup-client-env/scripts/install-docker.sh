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

echo "Install: Docker"
run sudo apt install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common
run bash -lc "curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg"
run bash -lc "echo \"deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable\" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null"
run sudo apt update
run sudo apt install -y docker-ce docker-ce-cli containerd.io
run sudo usermod -aG docker $USER || true
echo "Note: 로그아웃 후 재로그인해야 docker 그룹 변경이 적용됩니다."
