# setup-client-env Agent

목적: `requirements.md`를 기반으로 개발 도구와 환경 설정을 정의하고, 각 설치 항목을 독립적인 스크립트로 구성하여 재현 가능한 개발 환경을 제공한다.

구성요소:
- `scripts/` 디렉터리: 각 설치 항목별 실행 스크립트 (각 스크립트는 `--dry-run` / `--run` 옵션 지원)
  - `install-system.sh` : 시스템 업데이트 및 필수 패키지 설치
  - `install-cpp.sh` : C++ 컴파일러 및 빌드 도구 설치(g++, cmake, gdb)
  - `install-docker.sh` : Docker 엔진 설치 및 사용자 그룹 설정
  - `install-vcpkg.sh` : `vcpkg` 설치 및 부트스트랩
  - `install-opencv.sh` : OpenCV 설치 (apt 또는 vcpkg 방식 주석 포함)
  - `install-qt.sh` : Qt 개발 패키지 설치
  - `install-vscode.sh` : VSCode 설치 및 권장 확장 설치
  - `install-gtest.sh` : Google Test 설치 안내(또는 vcpkg 설치)

- `setup-client-env.py` : (이미 포함) 스킬 마크다운의 bash 블록을 추출하여 실행 가능한 에이전트

운영 규칙:
- 모든 스크립트는 기본적으로 `--dry-run` 모드로 동작하여 실제 변경 없이 실행 예정 명령을 출력한다.
- 실제 설치는 `--run` 옵션으로 실행한다. 루트 권한이 필요한 작업은 `sudo`로 실행할 것.
- 스크립트는 Ubuntu(또는 WSL2의 Ubuntu)를 기준으로 작성되었다. 다른 배포판에서는 명령이 달라질 수 있다.

검증 절차:
1. 각 스크립트의 `--dry-run`을 실행하여 출력 확인
2. 문제가 없으면 `--run`으로 실행 (권한 필요)

파일 위치 예시:
- `client1/agents/setup-client-env/scripts/install-system.sh`
- `client1/agents/setup-client-env/scripts/install-cpp.sh`

작성일: 2026-07-07
