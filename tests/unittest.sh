#!/usr/bin/env bash

gcc ../ini.c -DINI_MAX_LINE=20 unittest_string.c -o unittest_string
./unittest_string > baseline_string.txt
rm -f unittest_string

gcc ../ini.c -DINI_USE_STACK=0 -DINI_MAX_LINE=20 -DINI_INITIAL_ALLOC=20 unittest_string.c -o unittest_heap_string
./unittest_heap_string > baseline_heap_string.txt
rm -f unittest_heap_string
