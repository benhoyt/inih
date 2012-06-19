/* inih -- unit tests

This works simply by dumping a bunch of info to standard output, which is
redirected to an output file (baseline_*.txt) and checked into the Subversion
repository. This baseline file is the test output, so the idea is to check it
once, and if it changes -- look at the diff and see which tests failed.

Here's how I produced the two baseline files (with Tiny C Compiler):

tcc -DINI_ALLOW_MULTILINE=1 ../ini.c -run unittest.c > baseline_multi.txt
tcc -DINI_ALLOW_MULTILINE=0 ../ini.c -run unittest.c > baseline_single.txt

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ini.h"

int User;
char Prev_section[50];

int dumper(void* user, const char* section, const char* name,
           const char* value)
{
    User = (int)user;
    if (strcmp(section, Prev_section)) {
        printf("... [%s]\n", section);
        strncpy(Prev_section, section, sizeof(Prev_section));
        Prev_section[sizeof(Prev_section) - 1] = '\0';
    }
    printf("... %s=%s;\n", name, value);

    return strcmp(name, "user")==0 && strcmp(value, "parse_error")==0 ? 0 : 1;
}

void parse(const char* fname) {
    static int u = 100;
    int e;

    *Prev_section = '\0';
    e = ini_parse(fname, dumper, (void*)u);
    printf("%s: e=%d user=%d\n", fname, e, User);
    u++;
}

int main(void)
{
    parse("no_file.ini");
    parse("normal.ini");
    parse("bad_section.ini");
    parse("bad_comment.ini");
    parse("user_error.ini");
    parse("multi_line.ini");
    parse("bad_multi.ini");
    parse("bom.ini");
    return 0;
}
