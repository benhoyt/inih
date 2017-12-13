#!/usr/bin/env bash

gcc ../ini.c unittest.c -o unittest_multi
./unittest_multi > baseline_multi.txt
rm -f unittest_multi

gcc ../ini.c -DINI_ALLOW_MULTILINE=0 unittest.c -o unittest_single
./unittest_single > baseline_single.txt
rm -f unittest_single

gcc ../ini.c -DINI_ALLOW_INLINE_COMMENTS=0 unittest.c -o unittest_disallow_inline_comments
./unittest_disallow_inline_comments > baseline_disallow_inline_comments.txt
rm -f unittest_disallow_inline_comments

gcc ../ini.c -DINI_STOP_ON_FIRST_ERROR=1 unittest.c -o unittest_stop_on_first_error
./unittest_stop_on_first_error > baseline_stop_on_first_error.txt
rm -f unittest_stop_on_first_error

gcc ../ini.c -DINI_HANDLER_LINENO=1 unittest.c -o unittest_handler_lineno
./unittest_handler_lineno > baseline_handler_lineno.txt
rm -f unittest_handler_lineno

gcc ../ini.c -DINI_MAX_LINE=20 unittest_string.c -o unittest_string
./unittest_string > baseline_string.txt
rm -f unittest_string

gcc ../ini.c -DINI_USE_STACK=0 -DINI_MAX_LINE=20 unittest.c -o unittest_no_stack
./unittest_no_stack > baseline_no_stack.txt
rm -f unittest_no_stack

gcc ../ini.c -DINI_USE_STACK=0 -DINI_ALLOW_REALLOC=1 -DINI_MAX_LINE=100 unittest.c -o unittest_no_stack_realloc
./unittest_no_stack_realloc > baseline_no_stack_realloc.txt
rm -f unittest_no_stack_realloc

gcc ../ini.c -DINI_USE_STACK=0 -DINI_ALLOW_REALLOC=1 -DINI_MAX_LINE=0 -DINI_INITIAL_ALLOC=30 unittest.c -o unittest_no_stack_realloc_unlimited
./unittest_no_stack_realloc_unlimited > baseline_no_stack_realloc_unlimited.txt
rm -f unittest_no_stack_realloc_unlimited

gcc ../ini.c -DINI_MAX_LINE=0 -DINI_USE_STACK=0 -DINI_ALLOW_REALLOC=1 unittest_string.c -o unittest_no_stack_string 
./unittest_no_stack_string > baseline_no_stack_string.txt
rm -f unittest_no_stack_string
