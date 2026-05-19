#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${repo_root}"

if ! command -v watchexec >/dev/null 2>&1; then
    echo "watcher.sh requires watchexec. Install it with: brew install watchexec" >&2
    exit 127
fi

watch_args=(
    --restart
    --quiet
    --clear=reset
    --postpone
    --stop-signal SIGTERM
    --stop-timeout 2s
    --watch app/main.c
    --watch src/binary_io.h
    --watch src/binary_io.c
    --ignore 'build/**'
    --ignore '**/*.swp'
    --ignore '**/*.swo'
    --ignore '**/*~'
    --ignore '**/.DS_Store'
)

exec watchexec "${watch_args[@]}" -- ./run.sh "$@"
