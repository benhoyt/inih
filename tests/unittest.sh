#!/usr/bin/env bash

CC="${CC:-gcc} -Wall"

$CC ../ini.c -DINI_MAX_LINE=20 unittest.c -o unittest_multi_max_line
./unittest_multi_max_line > baseline_multi_max_line.txt
rm -f unittest_multi_max_line
