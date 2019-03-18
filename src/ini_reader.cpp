// Read an INI file into easy-to-access name/value pairs.

// inih and ini_reader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#include <string_view>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "ini.h"
#include "../inih.h"

namespace inih {
	using std::string;
	using std::string_view;

	// dbj
	string make_key(string_view section, string_view name) /*const*/
	{
		string key(section.data()); key.append("="); key.append(name.data());

#ifdef DBJ_INI_READER_NOT_CASE_SENSITIVE
		// Convert to lower case to make section/name lookups case-insensitive
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
#endif		
		return key;
	}

	struct ini_reader final : I_ini_reader
	{
	ini_reader(string_view filename)
	{
		_error = ini_parse(filename.data(), & ValueHandler, this);
	}

	int parse_error() const
	{
		return _error;
	}

	string get(string_view section, string_view name, string_view default_value) const
	{
		string key = make_key(section, name);
		// Use _values.find() here instead of _values.at() to support pre C++11 compilers
		return _values.count(key) ? _values.find(key)->second : default_value.data();
	}

	string get_string(string_view section, string_view name, string_view default_value) const
	{
		const string str = get(section, name, "");
		return str.empty() ? default_value.data() : str;
	}

	long get_integer(string_view section, string_view name, long default_value) const
	{
		string valstr = get(section, name, "");
		const char* value = valstr.c_str();
		char* end;
		// This parses "1234" (decimal) and also "0x4D2" (hex)
		long n = strtol(value, &end, 0);
		return end > value ? n : default_value;
	}

	double get_real(string_view section, string_view name, double default_value) const
	{
		string valstr = get(section, name, "");
		const char* value = valstr.c_str();
		char* end;
		double n = strtod(value, &end);
		return end > value ? n : default_value;
	}

	bool get_bool(string_view section, string_view name, bool default_value) const
	{
		string valstr = get(section, name, "");
		// Convert to lower case to make string comparisons case-insensitive
		std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
		if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
			return true;
		else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
			return false;
		else
			return default_value;
	}

	bool has_value(string_view section, string_view name) const
	{
		string key = make_key(section, name);
		return _values.count(key);
	}

	// dbj made into static
	static int ValueHandler(void* user, const char* section, const char* name,
		const char* value)
	{
		ini_reader* reader = static_cast<ini_reader*>(user);
		string key = make_key(section, name);
		if (reader->_values[key].size() > 0)
			reader->_values[key] += "\n";
		reader->_values[key] += value;
		return 1;
	}

		private:
			int _error;
			// dbj: authors idea is:
			// map key = ini section name + "=" + ini section key
			std::map<std::string, std::string> _values;
	}; // ini_reader

	I_ini_reader const & ini_reader_instance(string_view ini_file_name)
	{
		static ini_reader singleton_(ini_file_name);
		return singleton_;
	}

} // inih