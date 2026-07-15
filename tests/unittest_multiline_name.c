/* inih -- test that multiline values preserve long names */

#include <stdio.h>
#include <string.h>

#include "../ini.h"

#define NAME_49 "0123456789012345678901234567890123456789012345678"
#define NAME_50 "01234567890123456789012345678901234567890123456789"

#ifdef TEST_CUSTOM_LIMITS
#if INI_MAX_SECTION < 51 || INI_MAX_NAME < 51
#error "INI_MAX_SECTION and INI_MAX_NAME must include the NUL terminator"
#endif
#define TEST_SECTION NAME_50
#define TEST_NAME NAME_50
#else
#if INI_MAX_SECTION != 50 || INI_MAX_NAME != 50
#error "default section or name buffer size changed"
#endif
#define TEST_SECTION "section"
#define TEST_NAME NAME_49
#endif

typedef struct {
    int calls;
    int valid;
} capture_context;

static int capture(void* user, const char* section, const char* name,
                   const char* value)
{
    capture_context* context = (capture_context*)user;
    const char* expected_value = context->calls == 0 ? "first" : "second";

    context->calls++;
    if (strcmp(section, TEST_SECTION) != 0 || strcmp(name, TEST_NAME) != 0 ||
        strcmp(value, expected_value) != 0) {
        context->valid = 0;
    }
    return 1;
}

int main(void)
{
    const char* config = "[" TEST_SECTION "]\n" TEST_NAME "=first\n"
                         "  second\n";
    capture_context context = {0, 1};
    int error = ini_parse_string(config, capture, &context);

    if (error != 0 || context.calls != 2 || !context.valid) {
        fprintf(stderr, "long multiline name was not preserved\n");
        return 1;
    }
    return 0;
}
