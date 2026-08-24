#include <stdio.h>

#include "../ini.h"

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    (void)user;
    (void)section;
    (void)name;
    (void)value;
    return 1;
}

int main(void)
{
    const char* filename = "unittest_file_error.tmp";
    FILE* file = fopen(filename, "w");
    int error;
    int read_error;

    if (!file)
        return 1;

    error = ini_parse_file(file, handler, NULL);
    read_error = ferror(file);
    fclose(file);
    remove(filename);

    return read_error && error == -3 ? 0 : 2;
}
