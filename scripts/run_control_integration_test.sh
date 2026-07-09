#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/.." && pwd)
PY=${PY:-python3}
SERVER="$ROOT/scripts/mock_control_server.py"
CLIENT="$ROOT/scripts/control_integration_client.py"

echo "Starting mock server..."
$PY "$SERVER" &
SERVER_PID=$!
sleep 0.5

echo "Running integration client..."
$PY "$CLIENT" "rtsp://127.0.0.1/stream/1" || RC=$?
RC=${RC:-0}

if [ "$RC" -eq 0 ] && [ -x "$ROOT/build/control_network_smoke" ]; then
  echo "Running C++ control network smoke..."
  "$ROOT/build/control_network_smoke" || RC=$?
  RC=${RC:-0}
fi

echo "Stopping server (PID $SERVER_PID)"
kill $SERVER_PID 2>/dev/null || true
wait $SERVER_PID 2>/dev/null || true

if [ "$RC" -ne 0 ]; then
  echo "Integration test failed with code $RC"
  exit $RC
fi

echo "Integration test passed"
exit 0
