#!/bin/sh

set -eu

cd "$(dirname "${1}")"
{ "$2" || echo "exit status $?"; } | diff "${1}" -
