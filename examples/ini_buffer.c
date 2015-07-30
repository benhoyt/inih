/* Example: parse an INI file using a custom ini_reader and a string buffer for I/O

This example was provided by Grzegorz Sokół (https://github.com/greg-sokol)
in pull request https://github.com/benhoyt/inih/pull/38
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ini.h"

typedef struct
{
    int version;
    const char* name;
    const char* email;
} configuration;

typedef struct
{
    const char* ptr;
    int bytes_left;
} buffer_ctx;

static char* ini_buffer_reader(char* str, int num, void* stream)
{
    buffer_ctx* ctx = (buffer_ctx*)stream;
    int idx = 0;
    char newline = 0;

    if (ctx->bytes_left <= 0)
        return NULL;

    for (idx = 0; idx < num - 1; ++idx)
    {
        if (idx == ctx->bytes_left)
            break;

        if (ctx->ptr[idx] == '\n')
            newline = '\n';
        else if (ctx->ptr[idx] == '\r')
            newline = '\r';

        if (newline)
            break;
    }

    memcpy(str, ctx->ptr, idx);
    str[idx] = 0;

    ctx->ptr += idx + 1;
    ctx->bytes_left -= idx + 1;

    if (newline && ctx->bytes_left > 0 &&
            ((newline == '\r' && ctx->ptr[0] == '\n') ||
             (newline == '\n' && ctx->ptr[0] == '\r'))) {
        ctx->bytes_left--;
        ctx->ptr++;
    }
    return str;
}

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("protocol", "version")) {
        pconfig->version = atoi(value);
    } else if (MATCH("user", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("user", "email")) {
        pconfig->email = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

int main(int argc, char* argv[])
{
    configuration config;
    buffer_ctx ctx;

    ctx.ptr = "; Test ini buffer\n"
              "\n"
              "[protocol]\n"
              "version=42\n"
              "\n"
              "[user]\n"
              "name = Jane Smith\n"
              "email = jane@smith.com\n";
    ctx.bytes_left = strlen(ctx.ptr);

    if (ini_parse_stream((ini_reader)ini_buffer_reader, &ctx, handler, &config) < 0) {
        printf("Can't load buffer\n");
        return 1;
    }
    printf("Config loaded from buffer: version=%d, name=%s, email=%s\n",
        config.version, config.name, config.email);
    return 0;
}
