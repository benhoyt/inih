#!/usr/bin/env bash
clang -g -O1 -fsanitize=fuzzer,address lib-inihfuzz.c ../ini.c -o lib-inihfuzz
