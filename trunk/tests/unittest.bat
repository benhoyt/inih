@call tcc ..\ini.c -I..\ -run unittest.c > baseline_multi.txt
@call tcc ..\ini.c -I..\ -DINI_ALLOW_MULTILINE=0 -run unittest.c > baseline_single.txt
