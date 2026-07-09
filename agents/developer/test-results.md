# Developer Test Results

## Sprint 2

- Test date: 2026-07-09
- Environment: Ubuntu/WSL, g++, CMake Debug build

### Implemented

- Added `streaming` module:
  - `IFrameSource` interface for future OpenCV/FFmpeg RTSP sources
  - `StreamClient` playback state machine
  - retry policy for stream open failures
  - stream error codes and recommended user actions
  - `MockFrameSource` for deterministic tests
- Added `streaming_test` to CMake/CTest.

### Test Commands

```sh
./scripts/run_tests.sh
./scripts/run_control_integration_test.sh
```

### Results

- Unit/build test: PASSED
  - `control_test`: passed
  - `streaming_test`: passed
- Integration regression test: PASSED
  - Python Control Protocol smoke: passed
  - C++ `ControlClient` network smoke: passed

### Remaining Sprint 2 Scope

- Real RTSP/H.264 frame source is not implemented yet.
- Qt rendering surface is not implemented yet.

---

- Test date: 2026-07-09
- Environment: Ubuntu/WSL, g++, CMake Debug build
- Sprint: Sprint 1

## Implemented

- Expanded `ControlClient` from a stub into a Sprint 1 control core:
  - RTSP URL validation
  - connection state tracking
  - connect/disconnect operations
  - playback settings validation and `set_param` command support
  - standardized operation result, error code, recommended action, and console log output
  - optional JSON-over-TCP transport for the Control Protocol
- Added vcpkg toolchain auto-detection from `VCPKG_ROOT` in `CMakeLists.txt`.
- Added `control_network_smoke` executable for C++ Control Protocol integration verification.

## Test Commands

```sh
./scripts/run_tests.sh
./scripts/run_control_integration_test.sh
```

## Results

- Unit/build test: PASSED
  - `control_test`: 1/1 passed
- Integration test: PASSED
  - Python protocol client received `status: 200` and `session_id: s-mock-12345`
  - C++ `ControlClient` network smoke received `session_id: s-mock-12345`

## Remaining Sprint 1 Scope

- Qt UI screens are not implemented yet.
- RTSP/H.264 decode/render pipeline is not implemented yet.
- Current integration server is a mock Control Protocol server, not a real camera server.
