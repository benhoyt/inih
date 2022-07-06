/* This is a slightly tweaked copy of tests/unittest_string for libfuzzer fuzzing */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ini.h"

int User;
char Prev_section[10240];

int dumper(void* user, const char* section, const char* name,
           const char* value)
{
    User = *((int*)user);
    if (strcmp(section, Prev_section)) {
        //printf("... [%s]\n", section);
        strncpy(Prev_section, section, sizeof(Prev_section));
        Prev_section[sizeof(Prev_section) - 1] = '\0';
    }
    //printf("... %s=%s;\n", name, value);
    return 1;
}

void parse(const char* name, const char* string) {
    static int u = 100;
    int e;

    *Prev_section = '\0';
    e = ini_parse_string(string, dumper, &u);
    //printf("%s: e=%d user=%d\n", name, e, User);
    u++;
}

extern int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {

    if(Size < 1 || Size > 10240){
		return 0;
	}
	
	char brrr[10240];

	memcpy(brrr,Data,Size);

    parse("Fuzzer", brrr);

	return 0;
}

