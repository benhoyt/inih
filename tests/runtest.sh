#!/bin/sh

set -eu

cd "$(dirname "${1}")"
"$2" | diff "${1}" -
