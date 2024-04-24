#!/usr/bin/env bash

# Compile the fuzzer binary for oss-fuzz infrastructure.
$CC $CFLAGS -c ini.c
$CC $CFLAGS -c fuzzing/inihfuzz.c
$CXX $CFLAGS $LIB_FUZZING_ENGINE inihfuzz.o ini.o -o inihfuzz

# Setup for oss-fuzz infrastructure.
cp inihfuzz $OUT/
zip -r inihfuzz_seed_corpus.zip tests/*.ini
mv inihfuzz_seed_corpus.zip $OUT/
