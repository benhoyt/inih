/* inih -- tests with custom memory allocator */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif
#include "../ini.h"

void* ini_malloc(size_t size) {
    printf("ini_malloc(%d)\n", (int)size);
    return malloc(size);
}

void ini_free(void* ptr) {
    printf("ini_free()\n");
    free(ptr);
}

void* ini_realloc(void* ptr, size_t size) {
    printf("ini_realloc(%d)\n", (int)size);
    return realloc(ptr, size);
}

char Prev_section[50];

int dumper(void* user, const char* section, const char* name,
           const char* value)
{
    if (strcmp(section, Prev_section)) {
        printf("... [%s]\n", section);
        strncpy(Prev_section, section, sizeof(Prev_section));
        Prev_section[sizeof(Prev_section) - 1] = '\0';
    }
    printf("... %s=%s;\n", name, value);
    return 1;
}

void parse(const char* name, const char* string) {
    int e;

    *Prev_section = '\0';
    e = ini_parse_string(string, dumper, NULL);
    printf("%s: e=%d\n", name, e);
}

int main(void)
{
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    parse("basic", "[section]\nfoo = bar\nbazz = buzz quxx");
    return 0;
}
