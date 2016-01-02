@call tcc ..\ini.c -I..\ -run unittest.c > baseline_multi.txt
@call tcc ..\ini.c -I..\ -DINI_ALLOW_MULTILINE=0 -run unittest.c > baseline_single.txt
@call tcc ..\ini.c -I..\ -DINI_ALLOW_INLINE_COMMENTS=0 -run unittest.c > baseline_disallow_inline_comments.txt
@call tcc ..\ini.c -I..\ -DINI_STOP_ON_FIRST_ERROR=1 -run unittest.c > baseline_stop_on_first_error.txt
