# Product Backlog

기본 원칙: 각 항목은 EPIC/Story/Task로 분류되며, 우선순위(P0:Highest, P1, P2), 스토리 포인트(1/2/3/5/8), 예상 의존성을 함께 표기한다.

## EPIC: Core Streaming & Control

- Story: 서버 접속/접속 해제 기능 (P0, 3pts)
  - 설명: 사용자는 서버에 접속하고 접속 해제할 수 있어야 한다.
  - 수용 기준:
    - UI에서 서버 주소 입력 후 접속 명령 전송 가능
    - 접속 성공/실패에 대한 명확한 상태 표시
  - 의존성: 네트워크 모듈, 로깅

- Story: RTSP/H.264 스트리밍 수신 및 재생 (P0, 5pts)
  - 설명: 서버가 송출하는 RTSP/H.264 스트림을 수신하여 재생한다.
  - 수용 기준:
    - RTSP 스트림을 수신하여 UI에서 실시간 재생
    - 동영상 출력 지연이 허용 범위 내(프로토콜/네트워크 한계)
  - 의존성: OpenCV 또는 FFmpeg 바인딩, 네트워크

- Story: 프레임율 및 해상도 제어 (P1, 3pts)
  - 설명: 사용자가 재생 중 프레임레이트 또는 해상도를 변경할 수 있다.
  - 수용 기준:
    - UI에서 설정 변경 가능
    - 변경 사항이 재생에 반영되거나 사용자에게 안내 메시지 제공
  - 의존성: 스트리밍 파이프라인

- Story: 명령 송출(화면 제어 포함) (P1, 3pts)
  - 설명: 클라이언트가 서버에 제어 명령(접속, 해제, 파라미터 변경 등)을 전송한다.
  - 수용 기준:
    - 명령 전송 후 서버 응답을 로그에 기록
  - 의존성: 통신 프로토콜

- Story: 에러 처리 및 사용자 알림 (P0, 2pts)
  - 설명: 오류 발생 시 화면에 에러 메시지를 표시하고 로그를 남긴다.
  - 수용 기준:
    - 네트워크 장애, 스트림 파싱 오류 등 사례별 에러 메시지 표시

## EPIC: UI / UX

- Story: Qt 기반 UI 기본 레이아웃 및 상태 표시 (P0, 3pts)
  - 설명: 접속 상태, 스트리밍 화면, 설정 패널(프레임/해상도) 구현
  - 수용 기준:
    - 주요 컨트롤(접속/해제, 설정) 접근 가능

- Story: 에러/상태 메시지 표준화 (P1, 2pts)
  - 설명: 모든 오류와 상태 메시지는 일관된 형식으로 표시된다.

## EPIC: Performance & Reliability

- Story: 재시도 정책 및 네트워크 오류 핸들링 (P0, 3pts)
  - 설명: 네트워크 오류 발생 시 자동 재시도 또는 사용자 안내

- Story: 실시간 성능 모니터링(프레임율, 지연) (P2, 3pts)
  - 설명: 실시간으로 프레임율 및 지연을 모니터링하고 로그에 기록

## EPIC: Build, DevOps & Tooling

- Story: CMake 기반 빌드 스크립트 구성 (P0, 3pts)
  - 설명: 프로젝트가 CMake로 빌드되며 `Debug/Release`를 지원한다.

- Story: vcpkg 연동 및 외부 라이브러리 관리 (P0, 2pts)
  - 설명: OpenCV, gtest 등 의존성을 vcpkg로 설치/관리한다.

- Story: Dockerfile 및 컨테이너화 (P1, 3pts)
  - 설명: 개발/배포용 Dockerfile 제공

- Story: VSCode 설정 및 확장 추천 적용 (P1, 1pt)
  - 설명: `.vscode` 설정 및 확장(추천) 파일 추가

## EPIC: Testing & QA

- Story: Google Test 기반 단위/통합 테스트 작성 (P0, 5pts)
  - 설명: 핵심 모듈(연결관리, 스트림 파서, 에러 처리) 테스트 케이스 작성

- Story: CI 파이프라인 구성 (GitHub Actions) (P1, 3pts)
  - 설명: 빌드 및 테스트 자동화 파이프라인 구현

## EPIC: Security & Monitoring

- Story: 기본 네트워크 보안 가이드라인 적용(권장) (P2, 2pts)
  - 설명: 인증/암호화 적용 검토 및 문서화

- Story: 로그 수집 및 장애 모니터링(간단) (P2, 3pts)
  - 설명: 로깅 표준과 간단한 상태 모니터링 도구 연동

## EPIC: Future / Stretch

- Story: 멀티-카메라 동시 보기 (P2, 5pts)
- Story: 영상 녹화 및 재생 기능 (P2, 5pts)
- Story: 대역폭 제어 및 적응형 스트리밍(ABR) (P2, 5pts)

---
Generated from `requirements.md` on 2026-07-07
