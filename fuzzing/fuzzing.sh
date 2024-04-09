#!/usr/bin/env bash

CC=clang
CXX=clang++
LIB_FUZZING_ENGINE="-fsanitize=fuzzer"

# Compile and link with AddressSanitizer
CFLAGS_ASAN="-O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -fsanitize=address -fsanitize-address-use-after-scope -fsanitize=fuzzer-no-link"
$CC $CFLAGS_ASAN -c ../ini.c
$CC $CFLAGS_ASAN -c inihfuzz.c
$CXX $CFLAGS_ASAN $LIB_FUZZING_ENGINE inihfuzz.o ini.o -o inihfuzz_asan
rm *.o

# Compile and link with MemorySanitizer
CFLAGS_MSAN="-O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -fsanitize=memory -fsanitize-memory-track-origins -fsanitize=fuzzer-no-link"
$CC $CFLAGS_MSAN -c ../ini.c
$CC $CFLAGS_MSAN -c inihfuzz.c
$CXX $CFLAGS_MSAN $LIB_FUZZING_ENGINE inihfuzz.o ini.o -o inihfuzz_msan
rm *.o

# Compile and link with UndefinedBehaviorSanitizer
CFLAGS_UBSAN="-O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -fsanitize=array-bounds,bool,builtin,enum,float-divide-by-zero,function,integer-divide-by-zero,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,unsigned-integer-overflow,unreachable,vla-bound,vptr -fno-sanitize-recover=array-bounds,bool,builtin,enum,float-divide-by-zero,function,integer-divide-by-zero,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,unreachable,vla-bound,vptr -fsanitize=fuzzer-no-link"
$CC $CFLAGS_UBSAN -c ../ini.c
$CC $CFLAGS_UBSAN -c inihfuzz.c
$CXX $CFLAGS_UBSAN $LIB_FUZZING_ENGINE inihfuzz.o ini.o -o inihfuzz_ubsan
rm *.o

# Uncomment to run the fuzzer of your choice
mkdir covearge/
#./inihfuzz_asan  covearge/  testcases_seed/
#./inihfuzz_msan  covearge/  testcases_seed/
#./inihfuzz_ubsan covearge/  testcases_seed/
