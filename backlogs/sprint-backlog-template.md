# Sprint Backlog Template

## Sprint 정보
- Sprint 이름: Sprint 1
- 기간: YYYY-MM-DD ~ YYYY-MM-DD
- 목표: 첫 번째 스프린트에서 안정적인 스트리밍 재생과 기본 연결 제어 기능을 구현한다.

## 스프린트 백로그
- [ ] 서버 접속/접속 해제 기능 구현
- [ ] RTSP/H.264 스트리밍 수신 및 기본 재생 구현
- [ ] Qt 기반 UI에 접속/해제 버튼 및 상태 표시 추가
- [ ] 에러 메시지와 로그 표시 기능 구현
- [ ] CMake 빌드 구성 및 vcpkg 연동 검증

## 추가 태스크
- [ ] 개발 환경 설치 스크립트 검증
- [ ] GitHub Actions CI 템플릿 초기화
- [ ] Google Test 기본 테스트 케이스 추가

## 위험 요소
- RTSP 스트리밍 라이브러리 호환성 문제
- Qt UI와 OpenCV 재생 통합
- Docker/WSL2 환경에서 실행 이슈
