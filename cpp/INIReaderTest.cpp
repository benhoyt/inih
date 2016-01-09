// Example that shows simple usage of the INIReader class

#include <iostream>
#include <assert.h>
#include "INIReader.h"

std::map<const char*, bool (*)()> test_list;
INIReader reader("../examples/test.ini");

namespace expected {
    std::string _string = "@String!";
    int _number = 100;
    bool _boolean = true;
    double _double = 1.23456789;
}

bool test_get_long();
bool test_get_string();
bool test_get_boolean();
bool test_get_double();

int main()
{
    test_list["TestGetLong"] = &test_get_long;
    test_list["TestGetString"] = &test_get_string;
    test_list["TestGetBoolean"] = &test_get_boolean;
    test_list["TestGetDouble"] = &test_get_double;

    if (reader.ParseError() < 0) {
        std::cout << "Failed to load'test.ini'\n";
        return -1;
    }

    for (auto it = test_list.begin();it != test_list.end(); ++it) {
        bool test_result = it->second();
        std::cout << it->first << ": " << (test_result ? "Pass": "Fail") << '\n';
    }

    return 0;
}

bool test_get_long() {
    long readDefaultVersion = reader.GetLong(
            "invalid",
            "invalid",
            LONG_MAX
    );

    long readVersion = reader.GetLong(
            "tests",
            "number",
            -1
    );

    return (
            readVersion == expected::_number
            && readDefaultVersion == LONG_MAX
    );
}

bool test_get_string() {
    auto defaultValue = reader.Get(
            "invalid",
            "invalid",
            "DEFAULT"
    );

    auto readValue = reader.Get(
            "tests",
            "string",
            "UNKNOWN"
    );

    return (
            readValue == expected::_string
            && defaultValue.compare("DEFAULT") == 0
    );
}

bool test_get_boolean() {
    auto defaultValue = reader.GetBoolean(
            "invalid",
            "invalid",
            false
    );

    auto readValue = reader.GetBoolean(
            "tests",
            "boolean",
            false
    );

    return (
            readValue == expected::_boolean
            && !defaultValue
    );
}

bool test_get_double() {
    auto defaultValue = reader.GetReal(
            "invalid",
            "invalid",
            1.01
    );

    auto readValue = reader.GetReal(
            "tests",
            "double",
            1.01
    );

    return (
            readValue == expected::_double
            && defaultValue == 1.01
    );
}

