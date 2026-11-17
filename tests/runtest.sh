#!/bin/sh

set -eu

cd "$(dirname "${1}")"
output=$(mktemp)
trap 'rm -f "$output"' 0
trap 'exit 1' HUP INT TERM
status=0
"$2" > "$output" || status=$?
diff "${1}" "$output" || status=$?
exit "$status"
