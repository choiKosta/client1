# 프로젝트 개발 지침서 (instruction.md)

목적: `requirements.md`를 분석하여 프로젝트 전반에 적용할 **규칙**, **원칙**, **설정 템플레이트**를 정리한다. 이 파일은 개발자가 일관된 방식으로 코드를 작성·빌드·테스트·배포할 수 있도록 가이드한다.

---

## 1. 기본 원칙
- **언어/표준**: C++17 이상 사용을 권장한다(프로젝트 정책에 따라 C++20 선택 가능).
- **단일 책임**: 각 모듈은 단일 책임 원칙(SRP)을 따르도록 설계한다.
- **명확한 에러 처리**: 네트워크/스트리밍 오류는 사용자에게 명확한 메시지와 재시도/복구 방법을 제공해야 한다.
- **보안**: 비밀번호/토큰 등 비밀 정보는 소스에 저장하지 않고 환경 변수 또는 시크릿 매니저를 사용한다.

## 2. 코드 스타일 및 규칙
- **네이밍**: 클래스는 `PascalCase`, 함수/변수는 `camelCase` 또는 `snake_case` 중 하나를 프로젝트 전체에서 일관되게 사용한다.
- **변수명**: 의미 없는 한 글자 변수 사용 금지(루프 인덱스 i/j 제외).
- **헤더 가드**: 헤더에는 `#pragma once` 사용 권장.
- **메모리 안전**: 스마트 포인터(`std::unique_ptr`, `std::shared_ptr`) 우선 사용.
- **로그**: 중요한 작업(연결 시도/성공/실패, 재시도, 설정 변경)은 로깅한다. 로그 레벨(info/warn/error)을 사용한다.

## 3. 디렉터리 구조 권장
프로젝트의 최소 권장 구조:

```
project-root/
├─ CMakeLists.txt
├─ src/
│  ├─ main.cpp
│  └─ <modules>.cpp/.h
├─ include/
├─ tests/
│  └─ <gtest files>
├─ third_party/ (선택)
├─ docker/
│  └─ Dockerfile
├─ .vscode/
├─ docs/
└─ README.md
│
├─ agents
│   ├─ skills
│   ├─scripts
│   └─ references
└─ backlogs
```

## 4. 빌드 규칙 (CMake)
- **최소 요구**: 프로젝트는 `CMake` 기반으로 빌드되어야 한다.
- **빌드 타입**: `Debug`와 `Release` 지원
- **vcpkg 연동**: 외부 의존성은 vcpkg로 관리하고 `CMAKE_TOOLCHAIN_FILE`로 연동한다.

예시: `CMakeLists.txt` (루트)

```cmake
cmake_minimum_required(VERSION 3.16)
project(CameraClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT})
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain")
endif()

add_subdirectory(src)
add_subdirectory(tests)
```

`src/CMakeLists.txt` 예시:

```cmake
add_library(camera_core camera.cpp camera.h)
target_link_libraries(camera_core PRIVATE OpenCV::opencv)

add_executable(client_app main.cpp)
target_link_libraries(client_app PRIVATE camera_core Qt5::Widgets)
```

## 5. 테스트
- **프레임워크**: Google Test 사용
- **테스트 규칙**: 핵심 로직(연결 관리, 스트림 파싱, 에러 처리 등)에 유닛 테스트 작성

`tests/CMakeLists.txt` 예시:

```cmake
find_package(GTest REQUIRED)
add_executable(runTests test_main.cpp test_camera.cpp)
target_link_libraries(runTests PRIVATE GTest::gtest_main camera_core)
gtest_discover_tests(runTests)
```

## 6. VSCode 설정 (권장)
- 권장 확장: `ms-vscode.cpptools`, `ms-vscode.cmake-tools`, `twxs.cmake`, `ms-vscode-remote.remote-wsl` 등
- `.vscode/settings.json`에 CMake Tools 설정 및 형식화 규칙 포함(프로젝트에 이미 추가됨).

## 7. Docker 및 배포
- **Docker**: 개발/테스트용 Dockerfile을 제공한다. 컨테이너는 필요한 런타임(OpenCV, Qt runtime 관련 패키지 등)을 포함해야 한다.
- **이미지 태깅**: `projectname:latest`, CI에서는 `projectname:${{ github.sha }}` 형태 권장.

Dockerfile 템플릿:

```dockerfile
FROM ubuntu:22.04
RUN apt update && apt install -y ca-certificates libopencv-dev qtbase5-dev \
    && rm -rf /var/lib/apt/lists/*
COPY ./build/bin/client_app /usr/local/bin/
ENTRYPOINT ["/usr/local/bin/client_app"]
```

## 8. CI 템플레이트 (GitHub Actions)
간단한 CI 예시:

```yaml
name: CI
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Setup vcpkg
        run: |
          git clone https://github.com/microsoft/vcpkg.git $HOME/vcpkg
          $HOME/vcpkg/bootstrap-vcpkg.sh
      - name: Configure
        run: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake
      - name: Build
        run: cmake --build build --config Release -- -j 2
      - name: Run tests
        run: ctest --test-dir build --output-on-failure
```

## 9. Git 규칙 및 커밋 메시지
- **브랜치**: `main`(또는 `master`)는 릴리스 준비; 기능별 브랜치 `feature/<name>` 사용
- **커밋 메시지**: 요약(50자 이내) + 본문(필요 시). 예: `feat: add RTSP client connection retry`

## 10. 로깅·모니터링
- **로깅 포맷**: 타임스탬프, 레벨, 컴포넌트, 메시지
- **레벨**: DEBUG(개발), INFO(운영), WARN, ERROR

## 11. 템플레이트 예시 파일들
- `src/main.cpp` 기본 템플레이트:

```cpp
#include <iostream>
int main() {
  std::cout << "Camera Client starting...\n";
  return 0;
}
```

- `tests/test_main.cpp` (gtest 기본):

```cpp
#include <gtest/gtest.h>
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

---

작성자: 프로젝트 규칙 자동 생성
작성일: 2026-07-07
