#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

c_sources=(
    "${repo_root}/app/main.c"
    "${repo_root}/src/binary_io.c"
)

out="${repo_root}/build/c_scratch"
mkdir -p "${repo_root}/build"

build_log="$(mktemp "${TMPDIR:-/tmp}/dans_c_build.XXXXXX")"
cleanup() { rm -f "${build_log}"; }
trap cleanup EXIT

cflags=(-std=c89 -pedantic -Wall -Wextra -Wno-long-long -Werror)

if ! clang "${cflags[@]}" "${c_sources[@]}" -o "${out}" >"${build_log}" 2>&1; then
    cat "${build_log}" >&2
    exit 1
fi

exec "${out}" "$@"
