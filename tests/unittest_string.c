/* inih -- unit tests for ini_parse_string() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ini.h"

char Prev_section[50];
char output[1024];

int dumper(void* user, const char* section, const char* name,
           const char* value)
{
    char line[1024];
    if (strcmp(section, Prev_section)) {
	sprintf(line, "[%s]\n", section);
	strcat(output, line);
        strncpy(Prev_section, section, sizeof(Prev_section));
        Prev_section[sizeof(Prev_section) - 1] = '\0';
    }

    sprintf(line, "%s=%s;\n", name, value);
    strcat(output, line);
    return 1;
}

int parse(char* string, char* expected)
{
    int e;

    *Prev_section = '\0';
    *output = '\0';

    e = ini_parse_string(string, dumper, NULL);

    if (strcmp(output, expected)) {
	    printf("MISSMATCH!! \n-- got: '%s' \n-- expected: '%s'\n",
			    output, expected);
	    return -5;
    }

    return e;
}

int main(int argc, char* argv[])
{
    int ret;
    int exprc;

    if (argc < 4) {
	    printf("Missing parameters. try %s <input> <expected> <rc>\n", argv[0]);
	    return -1;
    }

    exprc = atoi(argv[3]);

    ret = parse(argv[1], argv[2]);
    if (ret)
        printf("parser return an error: %d\n", ret);
    if (ret != exprc)
        printf("unexpected RC: got %d, expected %d\n", ret, exprc);

    return (ret != exprc);
}
