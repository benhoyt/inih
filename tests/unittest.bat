@call tcc ..\ini.c -I..\ -run unittest.c > baseline_multi.txt
@call tcc ..\ini.c -I..\ -DINI_ALLOW_MULTILINE=0 -run unittest.c > baseline_single.txt
@call tcc ..\ini.c -I..\ -DINI_ALLOW_INLINE_COMMENTS=0 -run unittest.c > baseline_disallow_inline_comments.txt
@call tcc ..\ini.c -I..\ -DINI_STOP_ON_FIRST_ERROR=1 -run unittest.c > baseline_stop_on_first_error.txt
@call tcc ..\ini.c -I..\ -DINI_HANDLER_LINENO=1 -run unittest.c > baseline_handler_lineno.txt
@call tcc ..\ini.c -I..\ -DINI_USE_STACK=0 -DINI_MAX_LINE=20 -run unittest.c > baseline_no_stack.txt
@call tcc ..\ini.c -I..\ -DINI_USE_STACK=0 -DINI_ALLOW_REALLOC=1 -DINI_MAX_LINE=100 -run unittest.c > baseline_no_stack_realloc.txt
@call tcc ..\ini.c -I..\ -DINI_USE_STACK=0 -DINI_ALLOW_REALLOC=1 -DINI_MAX_LINE=0 -DINI_INITIAL_ALLOC=30 unittest.c > unittest_no_stack_realloc_unlimited.txt
