#!/bin/sh
# gdb may try to query debuginfod even when disabled from MI setup commands.
# Clearing DEBUGINFOD_URLS avoids debugger startup hangs in restricted WSL networks.
unset DEBUGINFOD_URLS
exec /usr/bin/gdb "$@"
