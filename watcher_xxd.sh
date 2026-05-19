#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${repo_root}"

if ! command -v watchexec >/dev/null 2>&1; then
    echo "cwatcher.sh requires watchexec. Install it with: brew install watchexec" >&2
    exit 127
fi

watch_args=(
    --restart
    --quiet
    --clear=reset
    --postpone
    --stop-signal SIGTERM
    --stop-timeout 2s
    --watch sample.dans
)

exec watchexec "${watch_args[@]}" -- 'sleep 0.4 && xxd sample.dans' "$@"
