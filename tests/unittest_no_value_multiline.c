#include <stdio.h>
#include <string.h>
#include "../ini.h"

#if INI_ALLOW_MULTILINE
#define CONT_NAME(name, value) name
#define CONT_VALUE(value) value
#else
#define CONT_NAME(name, value) value
#define CONT_VALUE(value) NULL
#endif

typedef struct {
    const char* section;
    const char* name;
    const char* value;
    int line;
} event;

typedef struct {
    const event* expected;
    size_t count;
    size_t seen;
    int reject_at;
    int failed;
} context;

static int equal(const char* actual, const char* expected)
{
    return expected ? actual && strcmp(actual, expected) == 0 : actual == NULL;
}

#if INI_HANDLER_LINENO
static int handler(void* user, const char* section, const char* name,
                   const char* value, int lineno)
#else
static int handler(void* user, const char* section, const char* name,
                   const char* value)
#endif
{
    context* ctx = (context*)user;
    int valid = ctx->seen < ctx->count;
    if (valid) {
        const event* expected = &ctx->expected[ctx->seen];
        valid = equal(section, expected->section) &&
            equal(name, expected->name) && equal(value, expected->value);
#if INI_HANDLER_LINENO
        valid = valid && lineno == expected->line;
#endif
    }
    if (!valid) {
        fprintf(stderr, "unexpected callback %lu: [%s] %s=%s\n",
            (unsigned long)ctx->seen, section, name ? name : "<NULL>",
            value ? value : "<NULL>");
        ctx->failed = 1;
    }
    return (int)ctx->seen++ != ctx->reject_at;
}

static int check(const char* label, const char* input, const event* expected,
                 size_t count, int expected_error, int reject_at)
{
    int method;
    int failed = 0;
    for (method = 0; method < 4; method++) {
        context ctx = {expected, count, 0, reject_at, 0};
        int error;
        if (method == 0)
            error = ini_parse_string(input, handler, &ctx);
        else if (method == 1)
            error = ini_parse_string_length(input, strlen(input), handler, &ctx);
        else {
            FILE* file = tmpfile();
            if (!file)
                return 1;
            if (fputs(input, file) == EOF || fseek(file, 0, SEEK_SET) != 0) {
                fclose(file);
                return 1;
            }
            if (method == 2)
                error = ini_parse_file(file, handler, &ctx);
            else
                error = ini_parse_stream((ini_reader)fgets, file, handler, &ctx);
            fclose(file);
        }
        if (error != expected_error || ctx.seen != count || ctx.failed) {
            fprintf(stderr, "%s method=%d error=%d callbacks=%lu expected=%lu\n",
                label, method, error, (unsigned long)ctx.seen, (unsigned long)count);
            failed = 1;
        }
    }
    return failed;
}

#define COUNT(events) (sizeof(events) / sizeof((events)[0]))

int main(void)
{
    static const event after_value[] = {
        {"", "old", "one", 1}, {"", "flag", NULL, 2},
        {"", CONT_NAME("flag", "two"), CONT_VALUE("two"), 3}
    };
    static const event first_name[] = {
        {"", "flag", NULL, 1},
        {"", CONT_NAME("flag", "two"), CONT_VALUE("two"), 2}
    };
    static const event consecutive[] = {
        {"", "first", NULL, 1}, {"", "second", NULL, 2},
        {"", CONT_NAME("second", "tail"), CONT_VALUE("tail"), 3}
    };
    static const event comments[] = {
        {"", "old", "one", 1},
#if INI_ALLOW_INLINE_COMMENTS
        {"", "flag", NULL, 2},
        {"", CONT_NAME("flag", "two"), CONT_VALUE("two"), 5}
#else
        {"", "flag ; note", NULL, 2},
        {"", CONT_NAME("flag ; note", "two ; note"), CONT_VALUE("two ; note"), 5}
#endif
    };
    static const event sections[] = {
        {"", "old", "one", 1},
#if INI_CALL_HANDLER_ON_NEW_SECTION
        {"next", NULL, NULL, 2},
#endif
        {"next", "flag", NULL, 3},
        {"next", CONT_NAME("flag", "tail"), CONT_VALUE("tail"), 4}
    };
    static const event ordinary[] = {
        {"", "old", "one", 1},
        {"", CONT_NAME("old", "two"), CONT_VALUE("two"), 2},
        {"", "next", "", 3},
        {"", CONT_NAME("next", "three"), CONT_VALUE("three"), 4}
    };
    char boundary_name[INI_MAX_NAME + 1];
    char boundary_saved[INI_MAX_NAME];
    char boundary_input[INI_MAX_NAME + 32];
    event boundary[] = {
        {"", "old", "one", 1}, {"", boundary_name, NULL, 2},
        {"", CONT_NAME(boundary_saved, "two"), CONT_VALUE("two"), 3}
    };
    size_t length;
    int failed = 0;
    failed |= check("after valued key", "old=one\nflag\n  two\n",
        after_value, COUNT(after_value), 0, -1);
    failed |= check("first key", "flag\n  two\n",
        first_name, COUNT(first_name), 0, -1);
    failed |= check("consecutive names", "first\nsecond\n  tail\n",
        consecutive, COUNT(consecutive), 0, -1);
    failed |= check("comments and blank lines",
        "old=one\nflag ; note\n; comment\n\n  two ; note\n",
        comments, COUNT(comments), 0, -1);
    failed |= check("section change", "old=one\n[next]\nflag\n  tail\n",
        sections, COUNT(sections), 0, -1);
    failed |= check("ordinary values", "old: one\n  two\nnext=\n  three",
        ordinary, COUNT(ordinary), 0, -1);
#if INI_STOP_ON_FIRST_ERROR
    failed |= check("handler rejection", "old=one\nflag\n  two\n",
        after_value, 2, 2, 1);
    failed |= check("after parse error", "[broken\nflag\n  two\n",
        NULL, 0, 1, -1);
#else
    failed |= check("handler rejection", "old=one\nflag\n  two\n",
        after_value, COUNT(after_value), 2, 1);
    failed |= check("after parse error", "[broken\nflag\n  two\n",
        after_value + 1, 2, 1, -1);
#endif
    for (length = INI_MAX_NAME - 1; length <= INI_MAX_NAME; length++) {
        memset(boundary_name, 'n', length);
        boundary_name[length] = '\0';
        memcpy(boundary_saved, boundary_name, INI_MAX_NAME - 1);
        boundary_saved[INI_MAX_NAME - 1] = '\0';
        sprintf(boundary_input, "old=one\n%s\n  two\n", boundary_name);
        failed |= check("name buffer boundary", boundary_input,
            boundary, COUNT(boundary), 0, -1);
    }
    return failed;
}
