// INIReader.h example that shows simple usage of the INIReader class

#include <iostream>
#include "INIReader.h"

using std::cout;

int main()
{
    INIReader reader("../examples/test.ini");

    if (reader.ParseError() < 0) {
        cout << "Couldn't read INI file!\n";
        return 1;
    }

    cout << "Valid config:";
    cout << " user_name='" << reader.Get("user", "name", "UNKNOWN") << "'";
    cout << " user_email='" << reader.Get("user", "email", "UNKNOWN") << "'";
    cout << " protocol_version="
         << reader.GetInteger("protocol", "version", -1);
    cout << "\n";

    return 0;
}
