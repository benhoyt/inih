#!/usr/bin/env bash

set -euo pipefail

cd "$(dirname "${1}")"
diff "${1}" <("${2}")
