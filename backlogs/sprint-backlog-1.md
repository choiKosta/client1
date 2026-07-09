# Sprint Backlog 1

## Sprint 정보
- Sprint 이름: Sprint 1
- 기간: 2026-07-07 ~ 2026-07-20
- 목표: 핵심 스트리밍 연결과 재생, 기본 UI 및 오류 처리 기능 구현

## Sprint Backlog
- [ ] 서버 접속/접속 해제 기능 구현
  - [ ] 서버 주소 입력 UI 구현
  - [ ] 접속/해제 명령 송수신 처리
  - [ ] 접속 상태 표시 및 응답 로그 구현
- [ ] RTSP/H.264 스트리밍 수신 및 재생 기본 구현
  - [ ] RTSP 스트림 수신 모듈 설정
  - [ ] 스트림 프레임 디코딩 및 렌더링 구현
  - [ ] 재생 오류 감지 및 재시도 로직 구현
- [ ] Qt 기반 UI에 접속/해제 버튼 및 상태 표시 추가
  - [ ] 메인 화면 레이아웃 구현
  - [ ] 설정 패널(프레임레이트/해상도) 구현
  - [ ] 상태/에러 메시지 표시 영역 구현
- [ ] 에러 처리/알림 및 로그 기록 기능 구현
  - [ ] 네트워크 오류 및 스트리밍 오류 분류
  - [ ] 사용자 알림 메시지 표시 구현
  - [ ] 로그 파일 또는 콘솔 로그 기록 구현
- [ ] CMake 빌드 구성 및 vcpkg 연동 확인
  - [ ] 루트 CMakeLists.txt 작성
  - [ ] vcpkg toolchain 연동 설정
  - [ ] Google Test 기본 테스트 빌드 확인

## 개발자 Handoff 체크리스트
- [x] 기능 구현 완료 (Control Protocol core)
- [x] 단위 테스트 실행 및 결과 기록
- [x] 코드 스타일 및 빌드 확인
- [x] `agents/developer/test-results.md`에 결과 작성

## 리뷰어 Handoff 체크리스트
- [x] 제출된 코드 리뷰
- [x] 통합 테스트 실행
- [x] 리뷰 결과 `agents/reviewer/review-results.md`에 작성
- [ ] Scrum Master에게 결과 보고

## 2026-07-09 진행 기록
- 완료: Control Protocol core 구현, 상태/오류/로그 처리, playback 설정 검증, vcpkg toolchain 자동 연동, C++ 네트워크 smoke 검증.
- 검증: `./scripts/run_tests.sh` 통과, `./scripts/run_control_integration_test.sh` 통과.
- 잔여: Qt UI 및 실제 RTSP/H.264 디코딩/렌더링 파이프라인은 의존성/앱 골격 추가 후 후속 Sprint에서 구현 필요.

## 우선순위
- P0: 서버 접속/해제, 스트리밍 재생, 에러 처리
- P1: UI 상태 표시, 빌드/테스트 환경 구성

## 위험 및 Assumptions
- RTSP 스트리밍 라이브러리 호환성 문제가 발생할 수 있음
- WSL2 또는 Ubuntu 환경에서 Qt/OpenCV 통합 이슈가 있을 수 있음
- 우선순위 상 WSL2 환경보다 Ubuntu 네이티브를 우선 테스트
