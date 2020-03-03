/* inih -- simple .INI file parser

SPDX-License-Identifier: BSD-3-Clause

Copyright (C) 2009-2020, Ben Hoyt

inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:

https://github.com/benhoyt/inih

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "ini.h"

#define MAX_SECTION 50
#define MAX_NAME 50

#ifndef INI_RUNTIME_OPTIONS
#define STATIC static const
#else
#define STATIC
#endif

STATIC bool ini_allow_multiline = INI_ALLOW_MULTILINE;
STATIC bool ini_allow_bom = INI_ALLOW_BOM;
STATIC char* ini_start_comment_prefixes = INI_START_COMMENT_PREFIXES;
STATIC bool ini_allow_inline_comments = INI_ALLOW_INLINE_COMMENTS;
STATIC char* ini_inline_comment_prefixes = INI_INLINE_COMMENT_PREFIXES;
STATIC bool ini_use_stack = INI_USE_STACK;
STATIC int ini_max_line = INI_MAX_LINE;
STATIC bool ini_allow_realloc = INI_ALLOW_REALLOC;
STATIC int ini_initial_alloc = INI_INITIAL_ALLOC;
STATIC bool ini_stop_on_first_error = INI_STOP_ON_FIRST_ERROR;
STATIC bool ini_call_handler_on_new_section = false;
STATIC bool ini_allow_no_value = INI_ALLOW_NO_VALUE;

/* Used by ini_parse_string() to keep track of string parsing state. */
typedef struct {
    const char* ptr;
    size_t num_left;
} ini_parse_string_ctx;

/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(const char* s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char*)s;
}

/* Return pointer to first char (of chars) or inline comment in given string,
   or pointer to null at end of string if neither found. Inline comment must
   be prefixed by a whitespace character to register as a comment. */
static char* find_chars_or_comment(const char* s, const char* chars)
{
    if (ini_allow_inline_comments) {
        int was_space = 0;
        while (*s && (!chars || !strchr(chars, *s)) &&
               !(was_space && strchr(ini_inline_comment_prefixes, *s))) {
            was_space = isspace((unsigned char)(*s));
            s++;
        }
    } else {
        while (*s && (!chars || !strchr(chars, *s))) {
            s++;
        }
    }
    return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
    return dest;
}

/* See documentation in header file. */
int ini_parse_stream(ini_reader reader, void* stream, ini_handler handler,
                     void* user)
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
    char line_buf[ini_use_stack ? ini_max_line : 0];
    char* line;
    size_t max_line;
    if (ini_use_stack) {
        line = line_buf;
        max_line = ini_max_line;
    } else {
        max_line = ini_initial_alloc;
    }
    char* new_line;
    size_t offset;
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";

    char* start;
    char* end;
    char* name;
    char* value;
    int lineno = 0;
    int error = 0;

    if (!ini_use_stack) {
        line = (char*)malloc(ini_initial_alloc);
        if (!line) {
            return -2;
        }
    }

#if INI_HANDLER_LINENO
#define HANDLER(u, s, n, v) handler(u, s, n, v, lineno)
#else
#define HANDLER(u, s, n, v) handler(u, s, n, v)
#endif

    /* Scan through stream line by line */
    while (reader(line, (int)max_line, stream) != NULL) {
        if (ini_allow_realloc && !ini_use_stack) {
            offset = strlen(line);
            while (offset == max_line - 1 && line[offset - 1] != '\n') {
                max_line *= 2;
                if (max_line > ini_max_line)
                    max_line = ini_max_line;
                new_line = realloc(line, max_line);
                if (!new_line) {
                    free(line);
                    return -2;
                }
                line = new_line;
                if (reader(line + offset, (int)(max_line - offset), stream) == NULL)
                    break;
                if (max_line >= ini_max_line)
                    break;
                offset += strlen(line + offset);
            }
        }

        lineno++;

        start = line;
        if (ini_allow_bom && lineno == 1 && (unsigned char)start[0] == 0xEF &&
                           (unsigned char)start[1] == 0xBB &&
                           (unsigned char)start[2] == 0xBF) {
            start += 3;
        }
        start = lskip(rstrip(start));

        if (strchr(ini_start_comment_prefixes, *start)) {
            /* Start-of-line comment */
        }
        else if (ini_allow_multiline && *prev_name && *start && start > line) {
            /* Non-blank line with leading whitespace, treat as continuation
               of previous name's value (as per Python configparser). */
            if (!HANDLER(user, section, prev_name, start) && !error)
                error = lineno;
        }
        else if (*start == '[') {
            /* A "[section]" line */
            end = find_chars_or_comment(start + 1, "]");
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
                if (ini_call_handler_on_new_section && !HANDLER(user, section, NULL, NULL) && !error)
                    error = lineno;
            }
            else if (!error) {
                /* No ']' found on section line */
                error = lineno;
            }
        }
        else if (*start) {
            /* Not a comment, must be a name[=:]value pair */
            end = find_chars_or_comment(start, "=:");
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = end + 1;
                if (ini_allow_inline_comments) {
                    end = find_chars_or_comment(value, NULL);
                    if (*end)
                        *end = '\0';
                }
                value = lskip(value);
                rstrip(value);

                /* Valid name[=:]value pair found, call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                if (!HANDLER(user, section, name, value) && !error)
                    error = lineno;
            }
            else if (!error) {
                /* No '=' or ':' found on name[=:]value line */
                if (ini_allow_no_value) {
                    *end = '\0';
                    name = rstrip(start);
                    if (!HANDLER(user, section, name, NULL) && !error)
                        error = lineno;
                } else {
                    error = lineno;
                }
            }
        }

        if (ini_stop_on_first_error && error)
            break;
    }

    if (!ini_use_stack) {
        free(line);
    }

    return error;
}

/* See documentation in header file. */
int ini_parse_file(FILE* file, ini_handler handler, void* user)
{
    return ini_parse_stream((ini_reader)fgets, file, handler, user);
}

/* See documentation in header file. */
int ini_parse(const char* filename, ini_handler handler, void* user)
{
    FILE* file;
    int error;

    file = fopen(filename, "r");
    if (!file)
        return -1;
    error = ini_parse_file(file, handler, user);
    fclose(file);
    return error;
}

/* An ini_reader function to read the next line from a string buffer. This
   is the fgets() equivalent used by ini_parse_string(). */
static char* ini_reader_string(char* str, int num, void* stream) {
    ini_parse_string_ctx* ctx = (ini_parse_string_ctx*)stream;
    const char* ctx_ptr = ctx->ptr;
    size_t ctx_num_left = ctx->num_left;
    char* strp = str;
    char c;

    if (ctx_num_left == 0 || num < 2)
        return NULL;

    while (num > 1 && ctx_num_left != 0) {
        c = *ctx_ptr++;
        ctx_num_left--;
        *strp++ = c;
        if (c == '\n')
            break;
        num--;
    }

    *strp = '\0';
    ctx->ptr = ctx_ptr;
    ctx->num_left = ctx_num_left;
    return str;
}

/* See documentation in header file. */
int ini_parse_string(const char* string, ini_handler handler, void* user) {
    ini_parse_string_ctx ctx;

    ctx.ptr = string;
    ctx.num_left = strlen(string);
    return ini_parse_stream((ini_reader)ini_reader_string, &ctx, handler,
                            user);
}
