// Example that demonstrates the ParseErrorMessage() method.

#include <iostream>
#include "../cpp/INIReader.h"

int main()
{
    INIReader reader_file_not_found("/file_that_does_not_exist.ini");
    INIReader reader_parse_error("../tests/name_only_after_error.ini");
    INIReader reader_success("../tests/normal.ini");

    std::cout
        << "reader_file_not_found errmsg: \"" << reader_file_not_found.ParseErrorMessage() << "\"\n"
        << "reader_parse_error errmsg: \"" << reader_parse_error.ParseErrorMessage() << "\"\n"
        << "reader_success errmsg: \"" << reader_success.ParseErrorMessage() << "\"\n";

    return 0;
}
