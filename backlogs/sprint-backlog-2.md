# Sprint Backlog 2

## Sprint 정보
- Sprint 이름: Sprint 2
- 기간: 2026-07-09 ~ 2026-07-22
- 목표: RTSP/H.264 재생을 위한 스트리밍 코어, 상태 전이, 재시도/오류 처리 기반 구현

## Sprint Backlog
- [x] 스트리밍 수신 모듈 인터페이스 구현
  - [x] `IFrameSource` 추상화 추가
  - [x] `StreamClient` 상태 전이 구현
  - [x] mock frame source 기반 테스트 가능 구조 구성
- [x] 재생 오류 감지 및 재시도 로직 구현
  - [x] open 실패 재시도 정책 구현
  - [x] retry limit 초과 오류 분류
  - [x] frame receive 실패 오류 분류
- [x] 스트리밍 상태/에러 메시지 표준화
  - [x] `StreamState` 추가
  - [x] `StreamErrorCode` 추가
  - [x] 사용자 권장 조치 메시지 포함
- [x] 단위 테스트 추가
  - [x] 정상 프레임 수신
  - [x] open 실패 후 재시도 성공
  - [x] retry limit 실패
  - [x] invalid RTSP URL 실패
  - [x] frame receive 실패
- [ ] 실제 RTSP/H.264 디코딩/렌더링 구현
  - [ ] OpenCV 또는 FFmpeg 기반 source 구현
  - [ ] Qt 렌더링 surface 연동

## 개발자 Handoff 체크리스트
- [x] 기능 구현 완료 (Streaming core)
- [x] 단위 테스트 실행 및 결과 기록
- [x] 코드 스타일 및 빌드 확인
- [x] `agents/developer/test-results.md`에 결과 작성

## 리뷰어 Handoff 체크리스트
- [x] 제출된 코드 리뷰
- [x] 통합 테스트 실행
- [x] 리뷰 결과 `agents/reviewer/review-results.md`에 작성
- [ ] Scrum Master에게 결과 보고

## 2026-07-09 진행 기록
- 완료: `streaming` 라이브러리, mock frame source, stream retry/error handling, `streaming_test` 추가.
- 검증: `./scripts/run_tests.sh` 통과, `./scripts/run_control_integration_test.sh` 통과.
- 잔여: 실제 RTSP/H.264 디코딩 및 Qt UI 렌더링은 외부 의존성 연동 후 구현 필요.

## 위험 및 Assumptions
- 실제 RTSP 구현은 OpenCV/FFmpeg 설치 상태에 따라 빌드 구성이 달라질 수 있다.
- WSL2 GUI 환경에서 Qt 렌더링 테스트는 별도 설정이 필요할 수 있다.
- Sprint 2는 재생 파이프라인의 테스트 가능한 코어를 우선 구현한다.
