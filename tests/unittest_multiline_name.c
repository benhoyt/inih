/* inih -- test that multiline values preserve long names */

#include <stdio.h>
#include <string.h>

#include "../ini.h"

#define LONG_NAME "01234567890123456789012345678901234567890123456789"

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
    if (strcmp(section, "section") != 0 || strcmp(name, LONG_NAME) != 0 ||
        strcmp(value, expected_value) != 0) {
        context->valid = 0;
    }
    return 1;
}

int main(void)
{
    const char* config = "[section]\n" LONG_NAME "=first\n"
                         "  second\n";
    capture_context context = {0, 1};
    int error = ini_parse_string(config, capture, &context);

    if (error != 0 || context.calls != 2 || !context.valid) {
        fprintf(stderr, "long multiline name was not preserved\n");
        return 1;
    }
    return 0;
}
