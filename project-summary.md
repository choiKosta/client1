# Project Summary

## 1. 개요
이 프로젝트는 `requirements.md` 기반 카메라 관제 클라이언트 개발을 위한 준비와 운영 체계를 정리한 저장소입니다.

주요 목표:
- Agent 중심 Scrum 프로세스 적용
- 개발 환경 및 도구 설치 자동화
- Product Backlog / Sprint Backlog 관리
- C++/CMake/Qt/OpenCV 기반 개발 환경 구성

## 2. 디렉터리 구조
```
client1/
├─ .vscode/
│  ├─ extensions.json
│  └─ settings.json
├─ agents/
│  ├─ README.md
│  ├─ scrum-master/
│  │  └─ scrum-master-agent.md
│  ├─ developer/
│  │  ├─ developer-agent.md
│  │  └─ test-results.md
│  ├─ reviewer/
│  │  ├─ reviewer-agent.md
│  │  └─ review-results.md
│  └─ setup-client-env/
│     ├─ setup-client-env.agent.md
│     ├─ setup-client-env.py
│     └─ scripts/
│        ├─ install-cpp.sh
│        ├─ install-docker.sh
│        ├─ install-gtest.sh
│        ├─ install-opencv.sh
│        ├─ install-qt.sh
│        ├─ install-system.sh
│        ├─ install-vcpkg.sh
│        └─ install-vscode.sh
├─ backlogs/
│  ├─ product-backlog.md
│  ├─ sprint-backlog-1.md
│  └─ sprint-backlog-template.md
├─ instruction.md
├─ requirements.md
└─ project-summary.md
```

## 3. Agent 구조
- `agents/setup-client-env/`
  - 개발 환경 설치 및 구성 자동화를 담당합니다.
  - `setup-client-env.py`는 스킬 문서의 bash 블록을 추출하여 실행할 수 있습니다.
  - `scripts/`에는 Ubuntu/WSL2 기준 설치 스크립트가 들어 있습니다.

- `agents/scrum-master/`
  - Scrum Master Agent 역할을 정의합니다.
  - Product Backlog 관리, Sprint 계획, 결과 검증 및 다음 Sprint 허가를 담당합니다.

- `agents/developer/`
  - Developer Agent 역할을 정의합니다.
  - Sprint Backlog를 기반으로 개발 작업을 수행하고 단위 테스트 결과를 기록합니다.

- `agents/reviewer/`
  - Reviewer Agent 역할을 정의합니다.
  - Developer Agent 결과를 검토하고 통합 테스트 결과를 기록합니다.

## 4. 백로그 구조
- `backlogs/product-backlog.md`: 요구사항 기반 Product Backlog
- `backlogs/sprint-backlog-1.md`: Sprint 1 실행 항목 및 Handoff 체크리스트
- `backlogs/sprint-backlog-template.md`: 향후 Sprint 백로그 작성 템플릿

## 5. 개발 프로세스 정의
1. Scrum Master Agent는 `requirements.md`를 분석하여 `backlogs/product-backlog.md`를 유지합니다.
2. Sprint Backlog는 우선순위에 따라 `backlogs/sprint-backlog-<sprint-name>.md`로 작성됩니다.
3. Developer Agent는 Sprint Backlog를 수령하여 구현 및 단위 테스트를 수행합니다.
4. Reviewer Agent는 Developer 결과를 검토하고 통합 테스트를 실행합니다.
5. Sprint 종료 후 Scrum Master Agent가 `agents/developer/test-results.md`와 `agents/reviewer/review-results.md`를 확인하여 결과를 검증합니다.
6. 문제가 발생하면 Product Backlog를 수정하고 반복합니다.
7. 다음 Sprint 진행은 Scrum Master Agent가 검증 후 허가합니다.

## 6. 실행 준비 상태
- 개발 환경 자동화 스크립트 준비 완료
- Agent 정의 및 문서화 완료
- Sprint 1 백로그 정의 완료
- 실제 코드/빌드 템플릿은 현재 추가 작업 대상입니다

## 7. 권장 다음 단계
- `src/`, `tests/`, `CMakeLists.txt` 등 실제 개발 코드 템플릿 추가
- Sprint 1 개발 실행 및 테스트 결과 기록
- Sprint 1 종료 시 결과 검증용 `backlogs/sprint-retrospective-1.md` 생성
