# VS Code Run and Debug Configuration

목적: `/home/kosta/lecture0/.vscode`의 Run and Debug 패턴을 기준으로 `client1` 프로젝트의 VS Code 실행/디버그 구성을 생성한다.

생성 항목:
- `.vscode/launch.json`
- `.vscode/tasks.json`
- `.vscode/gdb-wrapper.sh`

적용 내용:
- `control_test` 빌드/실행/디버그
- `streaming_test` 빌드/실행/디버그
- `control_network_smoke` 빌드/디버그
- Sprint 1 통합 테스트 실행
- GDB `debuginfod` 지연 방지 wrapper
- CMake Debug configure/build task

```bash
agents/setup-client-env/scripts/configure-vscode-debug.sh --run
```
