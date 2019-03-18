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

	// dbj added

	using hash_key_type = long;

	static hash_key_type djb2_hash(unsigned char *str)
	{
		unsigned long hash = 5381;
		int c;

		while (c = *str++)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
	}

	// dbj
	static hash_key_type make_key(string_view section, string_view name) /*const*/
	{
		string key(section.data()); key.append("="); key.append(name.data());

#ifdef DBJ_INI_READER_NOT_CASE_SENSITIVE
		// Convert to lower case to make section/name lookups case-insensitive
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
#endif		
		return djb2_hash((unsigned char *)key.c_str());
	}

	struct ini_reader final : I_ini_reader
	{

		ini_reader(string_view filename)
		{
			_error = ini_parse(filename.data(), &ValueHandler, this);
		}

		int parse_error() const
		{
			return _error;
		}

		smart_buffer get(string_view section, string_view name, string_view default_value) const
		{
			hash_key_type key = make_key(section, name);
			// smart_buffer rvl;
			// 
			return smart_buffer(
				(char*)_strdup(
				(kv_map_.count(key) ? kv_map_.find(key)->second.data() : default_value.data())
				)
			);
		}

		smart_buffer get_string(string_view section, string_view name, string_view default_value) const
		{
			smart_buffer str = get(section, name, "");
			return  (false == str ? smart_buffer(_strdup(default_value.data())) : str);
		}

		long get_integer(string_view section, string_view name, long default_value) const
		{
			smart_buffer valstr = get(section, name, "");
			const char* value = valstr.get();
			char* end;
			// This parses "1234" (decimal) and also "0x4D2" (hex)
			long n = strtol(value, &end, 0);
			return end > value ? n : default_value;
		}

		double get_real(string_view section, string_view name, double default_value) const
		{
			smart_buffer valstr = get(section, name, "");
			const char* value = valstr.get();
			char* end;
			double n = strtod(value, &end);
			return end > value ? n : default_value;
		}

		bool get_bool(string_view section, string_view name, bool default_value) const
		{
			auto eq = [](char const * left_, char const * right_) {
				return 0 == std::strcmp(left_, right_);
			};
			smart_buffer valstr = get(section, name, "");
			char const * val = valstr.get();
			// Convert to lower case to make string comparisons case-insensitive
			// std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
			if (eq(val, "true") || eq(val, "yes") || eq(val, "on") || eq(val, "1"))
				return true;
			else if (eq(val, "false") || eq(val, "no") || eq(val, "off") || eq(val, "0"))
				return false;
			else
				return default_value;

		}

		bool has_value(string_view section, string_view name) const
		{
			hash_key_type key = make_key(section, name);
			return kv_map_.count(key);
		}

		// dbj made into static
		static int ValueHandler(void* user, const char* section, const char* name,
			const char* value)
		{
			ini_reader* reader = static_cast<ini_reader*>(user);
			hash_key_type key = make_key(section, name);
			/*
			dbj: ok this was original authors idea to handle multiple ini file
			entries under the same key, in the same section...

			[section]
			key=A
			key=B
			key=C

			key is: "section=key"
			value made as "A\nB\nC"

			DBJ: what happens if value is empty string?

			*/
			if (reader->kv_map_.count(key)) 
			{
				string new_val(reader->kv_map_[key]);
				new_val.append("\n");
				new_val.append(value);
				reader->kv_map_[key] = new_val;
			}
			else {
				reader->kv_map_[key] = value;
			}
			return 1;
		}
#ifdef _DEBUG
		~ini_reader() {
			kv_map_.clear();
		}
#endif
	private:

		// dbj: here hide def. ctor and make this no copy, no move class
		ini_reader() = delete;
		ini_reader(ini_reader const & ) = delete;
		ini_reader & operator = (ini_reader const & ) = delete;
		ini_reader(ini_reader && ) = delete;
		ini_reader & operator = (ini_reader &&) = delete;

		int _error{};

		// dbj: authors idea was:
		// map key = ini section name + "=" + ini section key
		// dbj changed from:
		// std::map<std::string, std::string> kv_map_;
		// to:
		std::map<hash_key_type, std::string> kv_map_{};
	}; // ini_reader

	I_ini_reader const & ini_reader_instance(string_view ini_file_name)
	{
		static ini_reader singleton_(ini_file_name);
		return singleton_;
	}

} // inih