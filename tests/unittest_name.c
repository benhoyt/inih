/* inih -- property name boundary tests */

#include <stdio.h>
#include <string.h>
#include "../ini.h"

typedef struct {
    char name[128];
    unsigned int calls;
    int bare;
    int failed;
} name_check;

static int handler(void* user, const char* section, const char* name,
                   const char* value
#if INI_HANDLER_LINENO
                   , int lineno
#endif
                   )
{
    name_check* check = (name_check*)user;
    const char* expected_value;
    size_t i;
    (void)section;
#if INI_HANDLER_LINENO
    (void)lineno;
#endif
    for (i = 0; i < sizeof(check->name); i++) {
        if (!name || name[i] != check->name[i]) {
            check->failed = 1;
            break;
        }
        if (!check->name[i])
            break;
    }
    expected_value = check->calls ? "second" : "first";
    if (check->bare && !check->calls) {
        if (value)
            check->failed = 1;
    }
    else if (!value || strcmp(value, expected_value)) {
        check->failed = 1;
    }
    check->calls++;
    return 1;
}

static int check_name(size_t length, int bare, int continuation)
{
    char input[256];
    name_check check;
    size_t expected_length = length;
    size_t used = length;
    int result;
    memset(&check, 0, sizeof(check));
    check.bare = bare;
#if INI_ALLOW_MULTILINE
    if (expected_length >= INI_MAX_NAME)
        expected_length = INI_MAX_NAME - 1;
#endif
    memset(check.name, 'k', expected_length);
    memset(input, 'k', length);
    if (bare) {
        memcpy(input + used, " \n", sizeof(" \n"));
        used += sizeof(" \n") - 1;
    }
    else {
        memcpy(input + used, " =first\n", sizeof(" =first\n"));
        used += sizeof(" =first\n") - 1;
    }
    if (continuation)
        memcpy(input + used, " second\n", sizeof(" second\n"));
    result = ini_parse_string(input, handler, &check);
    if (result || check.failed || check.calls != (continuation ? 2U : 1U)) {
        fprintf(stderr, "name boundary failed: length=%u bare=%d continuation=%d result=%d calls=%u\n",
                (unsigned int)length, bare, continuation, result, check.calls);
        return 1;
    }
    return 0;
}

int main(void)
{
    static const size_t lengths[] = {1, 3, 4, 48, 49, 50, 60, 80};
    size_t i;
    unsigned int passed = 0;
    for (i = 0; i < sizeof(lengths) / sizeof(lengths[0]); i++) {
        if (check_name(lengths[i], 0, 0))
            return 1;
        passed++;
#if INI_ALLOW_NO_VALUE
        if (check_name(lengths[i], 1, 0))
            return 1;
        passed++;
#endif
#if INI_ALLOW_MULTILINE && INI_MAX_NAME > 1
        if (check_name(lengths[i], 0, 1))
            return 1;
        passed++;
#if INI_ALLOW_NO_VALUE
        if (check_name(lengths[i], 1, 1))
            return 1;
        passed++;
#endif
#endif
    }
    printf("name boundary checks: %u passed\n", passed);
    return 0;
}
