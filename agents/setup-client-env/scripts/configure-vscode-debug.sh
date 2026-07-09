#!/usr/bin/env bash
set -euo pipefail

DRY_RUN=true
for arg in "$@"; do
  case "$arg" in
    --run) DRY_RUN=false ;;
    --dry-run) DRY_RUN=true ;;
    -h|--help)
      echo "Usage: $0 [--dry-run|--run]"
      exit 0
      ;;
    *)
      echo "Unknown argument: $arg" >&2
      exit 2
      ;;
  esac
done

ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
VSCODE_DIR="$ROOT/.vscode"

write_file() {
  local target="$1"
  local content="$2"
  if [ "$DRY_RUN" = true ]; then
    echo "[DRY-RUN] write $target"
    return
  fi
  mkdir -p "$(dirname "$target")"
  printf "%s\n" "$content" > "$target"
  echo "Wrote $target"
}

LAUNCH_JSON='{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug control_test",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/control_test",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "${workspaceFolder}/.vscode/gdb-wrapper.sh",
            "miDebuggerArgs": "-iex \"set auto-load safe-path ${workspaceFolder}:/usr/share/gdb/auto-load\" -iex \"set debuginfod enabled off\"",
            "preLaunchTask": "Build control_test",
            "logging": {
                "engineLogging": true,
                "trace": true,
                "traceResponse": true
            },
            "setupCommands": [
                {
                    "description": "Disable debuginfod prompt",
                    "text": "set debuginfod enabled off",
                    "ignoreFailures": true
                },
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        },
        {
            "name": "Run control_test (no debugger)",
            "type": "node-terminal",
            "request": "launch",
            "command": "./build/control_test",
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build control_test"
        },
        {
            "name": "Debug streaming_test",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/streaming_test",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "${workspaceFolder}/.vscode/gdb-wrapper.sh",
            "miDebuggerArgs": "-iex \"set auto-load safe-path ${workspaceFolder}:/usr/share/gdb/auto-load\" -iex \"set debuginfod enabled off\"",
            "preLaunchTask": "Build streaming_test",
            "logging": {
                "engineLogging": true,
                "trace": true,
                "traceResponse": true
            },
            "setupCommands": [
                {
                    "description": "Disable debuginfod prompt",
                    "text": "set debuginfod enabled off",
                    "ignoreFailures": true
                },
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        },
        {
            "name": "Run streaming_test (no debugger)",
            "type": "node-terminal",
            "request": "launch",
            "command": "./build/streaming_test",
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build streaming_test"
        },
        {
            "name": "Debug control_network_smoke",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/control_network_smoke",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "${workspaceFolder}/.vscode/gdb-wrapper.sh",
            "miDebuggerArgs": "-iex \"set auto-load safe-path ${workspaceFolder}:/usr/share/gdb/auto-load\" -iex \"set debuginfod enabled off\"",
            "preLaunchTask": "Build control_network_smoke",
            "logging": {
                "engineLogging": true,
                "trace": true,
                "traceResponse": true
            },
            "setupCommands": [
                {
                    "description": "Disable debuginfod prompt",
                    "text": "set debuginfod enabled off",
                    "ignoreFailures": true
                },
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        },
        {
            "name": "Run Sprint 1 integration (no debugger)",
            "type": "node-terminal",
            "request": "launch",
            "command": "./scripts/run_control_integration_test.sh",
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "Build control_network_smoke"
        }
    ]
}'

TASKS_JSON='{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Configure client1",
            "type": "shell",
            "command": "cmake",
            "args": [
                "-S",
                "${workspaceFolder}",
                "-B",
                "${workspaceFolder}/build",
                "-DCMAKE_BUILD_TYPE=Debug"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": []
        },
        {
            "label": "Build client1",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/build"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "dependsOn": [
                "Configure client1"
            ],
            "dependsOrder": "sequence",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "Build control_test",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/build",
                "--target",
                "control_test"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "dependsOn": [
                "Configure client1"
            ],
            "dependsOrder": "sequence",
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "Build control_network_smoke",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/build",
                "--target",
                "control_network_smoke"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "dependsOn": [
                "Configure client1"
            ],
            "dependsOrder": "sequence",
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "Build streaming_test",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "${workspaceFolder}/build",
                "--target",
                "streaming_test"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "dependsOn": [
                "Configure client1"
            ],
            "dependsOrder": "sequence",
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "Run unit tests",
            "type": "shell",
            "command": "${workspaceFolder}/scripts/run_tests.sh",
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "group": "test",
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "Run control integration test",
            "type": "shell",
            "command": "${workspaceFolder}/scripts/run_control_integration_test.sh",
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "dependsOn": [
                "Build control_network_smoke"
            ],
            "dependsOrder": "sequence",
            "group": "test",
            "problemMatcher": [
                "$gcc"
            ]
        },
        {
            "label": "Check GDB ptrace permission",
            "type": "shell",
            "command": "gdb",
            "args": [
                "-q",
                "-batch",
                "-iex",
                "set auto-load safe-path ${workspaceFolder}:/usr/share/gdb/auto-load",
                "-iex",
                "set debuginfod enabled off",
                "-ex",
                "break main",
                "-ex",
                "run",
                "-ex",
                "bt",
                "${workspaceFolder}/build/control_test"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "dependsOn": [
                "Build control_test"
            ],
            "dependsOrder": "sequence",
            "problemMatcher": []
        }
    ]
}'

GDB_WRAPPER='#!/bin/sh
# gdb may try to query debuginfod even when disabled from MI setup commands.
# Clearing DEBUGINFOD_URLS avoids debugger startup hangs in restricted WSL networks.
unset DEBUGINFOD_URLS
exec /usr/bin/gdb "$@"'

write_file "$VSCODE_DIR/launch.json" "$LAUNCH_JSON"
write_file "$VSCODE_DIR/tasks.json" "$TASKS_JSON"
write_file "$VSCODE_DIR/gdb-wrapper.sh" "$GDB_WRAPPER"

if [ "$DRY_RUN" = false ]; then
  chmod +x "$VSCODE_DIR/gdb-wrapper.sh"
  python3 -m json.tool "$VSCODE_DIR/launch.json" >/dev/null
  python3 -m json.tool "$VSCODE_DIR/tasks.json" >/dev/null
  echo "VS Code run/debug configuration is ready."
fi
