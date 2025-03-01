// Example that shows simple usage of the INIReader class

#include <iostream>
#include "../cpp/INIReader.h"

int main()
{
    INIReader reader("../examples/test.ini");

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'test.ini'\n";
        return 1;
    }
    std::cout << "Config loaded from 'test.ini': version="
              << reader.GetInteger("protocol", "version", -1) << ", unsigned version="
              << reader.GetUnsigned("protocol", "version", -1) << ", trillion="
              << reader.GetInteger64("user", "trillion", -1) << ", unsigned trillion="
              << reader.GetUnsigned64("user", "trillion", -1) << ", name="
              << reader.Get("user", "name", "UNKNOWN") << ", email="
              << reader.Get("user", "email", "UNKNOWN") << ", pi="
              << reader.GetReal("user", "pi", -1) << ", active="
              << reader.GetBoolean("user", "active", true) << "\n";
    std::cout << "Has values: user.name=" << reader.HasValue("user", "name")
              << ", user.nose=" << reader.HasValue("user", "nose") << "\n";
    std::cout << "Has sections: user=" << reader.HasSection("user")
              << ", fizz=" << reader.HasSection("fizz") << "\n";
    
    std::cout << "Sections:\n";
    std::vector<std::string> sections = reader.Sections();
    for (std::vector<std::string>::const_iterator it = sections.begin(); it != sections.end(); ++it) {
        std::cout << "- " << *it << "\n";
    }
    
    for (std::vector<std::string>::const_iterator it = sections.begin(); it != sections.end(); ++it) {
        std::cout << "Keys in section [" << *it << "]:\n";
        std::vector<std::string> keys = reader.Keys(*it);
        for (std::vector<std::string>::const_iterator kit = keys.begin(); kit != keys.end(); ++kit) {
            std::cout << "- " << *kit << "\n";
        }
    }
    
    return 0;
}
