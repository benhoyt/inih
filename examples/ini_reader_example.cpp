// Example that shows simple usage of the ini_reader class

#include <cstdio>
#include "../inih.h"

int main()
{
	using namespace std;
	using namespace inih;

	I_ini_reader const & reader = ini_reader_instance("../examples/test.ini");

    if (reader.parse_error() < 0) {
        fprintf( stderr, "Can't load 'test.ini'\n");
        return 1;
    }
	printf("\n\nConfig loaded from 'test.ini': \nversion=%3d\nname=%16s\nemail=%16s\npi=%3.3f\nactive=%6s"
		, reader.get_integer("protocol", "version", -1) 
		, reader.get("user", "name", "UNKNOWN").c_str()
		, reader.get("user", "email", "UNKNOWN").c_str()
		, reader.get_real("user", "pi", -1) 
		, (reader.get_bool("user", "active", true) ? "true" : "false")
	);
    return 0;
}
