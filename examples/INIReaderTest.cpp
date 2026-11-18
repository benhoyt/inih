// Tests for INIReader integer getters.

#include <cerrno>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include "../cpp/INIReader.h"

namespace {

int failures = 0;

template<typename T>
std::string ToDecimal(T value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

template<typename T>
std::string ToHex(T value)
{
    std::ostringstream stream;
    stream << "0x" << std::hex << value;
    return stream.str();
}

std::string IncrementDecimal(std::string value)
{
    for (std::string::reverse_iterator it = value.rbegin(); it != value.rend(); ++it) {
        if (*it != '9') {
            ++*it;
            return value;
        }
        *it = '0';
    }
    return "1" + value;
}

std::string IncrementHex(std::string value)
{
    for (std::string::reverse_iterator it = value.rbegin(); it != value.rend(); ++it) {
        if (*it >= '0' && *it < '9') {
            ++*it;
            return value;
        }
        if (*it == '9') {
            *it = 'a';
            return value;
        }
        if (*it >= 'a' && *it < 'f') {
            ++*it;
            return value;
        }
        *it = '0';
    }
    return "1" + value;
}

template<typename T>
void Check(const std::string& name, T actual, T expected)
{
    if (actual != expected) {
        std::cout << name << ": got " << actual << ", expected " << expected << '\n';
        ++failures;
    }
}

}  // namespace

int main()
{
    const long long_default = -17;
    const int64_t int64_default = -18;
    const unsigned long ulong_default = 17;
    const uint64_t uint64_default = 18;

    const unsigned long long_min_magnitude =
        static_cast<unsigned long>(-(std::numeric_limits<long>::min() + 1)) + 1;
    const std::string ulong_max_decimal = ToDecimal(std::numeric_limits<unsigned long>::max());
    const std::string uint64_max_decimal = ToDecimal(std::numeric_limits<uint64_t>::max());
    const std::string ulong_max_hex = ToHex(std::numeric_limits<unsigned long>::max());
    const std::string uint64_max_hex = ToHex(std::numeric_limits<uint64_t>::max());

    std::string input = "[numbers]\n";
    input += "long_min=" + ToDecimal(std::numeric_limits<long>::min()) + "\n";
    input += "long_max=" + ToDecimal(std::numeric_limits<long>::max()) + "\n";
    input += "long_hex_min=-" + ToHex(long_min_magnitude) + "\n";
    input += "long_hex_max=" + ToHex(static_cast<unsigned long>(std::numeric_limits<long>::max())) + "\n";
    input += "long_over=" + ToDecimal(static_cast<unsigned long>(std::numeric_limits<long>::max()) + 1) + "\n";
    input += "long_under=-" + ToDecimal(long_min_magnitude + 1) + "\n";
    input += "long_hex_over=" + ToHex(static_cast<unsigned long>(std::numeric_limits<long>::max()) + 1) + "\n";
    input += "long_hex_under=-" + ToHex(long_min_magnitude + 1) + "\n";
    input += "long_decimal=42\nlong_hex=0x2a\nlong_empty=\nlong_trailing=123xyz\n";

    input += "int64_min=-9223372036854775808\nint64_max=9223372036854775807\n";
    input += "int64_hex_min=-0x8000000000000000\nint64_hex_max=0x7fffffffffffffff\n";
    input += "int64_over=9223372036854775808\nint64_under=-9223372036854775809\n";
    input += "int64_hex_over=0x8000000000000000\nint64_hex_under=-0x8000000000000001\n";
    input += "int64_decimal=42\nint64_hex=0x2a\nint64_empty=\nint64_trailing=123xyz\n";

    input += "ulong_zero=0\nulong_max=" + ulong_max_decimal + "\n";
    input += "ulong_hex_max=" + ulong_max_hex + "\n";
    input += "ulong_over=" + IncrementDecimal(ulong_max_decimal) + "\n";
    input += "ulong_hex_over=0x" + IncrementHex(ulong_max_hex.substr(2)) + "\n";
    input += "ulong_negative=-1\nulong_negative_zero=-0\n";
    input += "ulong_decimal=42\nulong_hex=0x2a\nulong_empty=\nulong_trailing=123xyz\n";

    input += "uint64_zero=0\nuint64_max=" + uint64_max_decimal + "\n";
    input += "uint64_hex_max=" + uint64_max_hex + "\n";
    input += "uint64_over=" + IncrementDecimal(uint64_max_decimal) + "\n";
    input += "uint64_hex_over=0x" + IncrementHex(uint64_max_hex.substr(2)) + "\n";
    input += "uint64_negative=-1\nuint64_negative_zero=-0\n";
    input += "uint64_decimal=42\nuint64_hex=0x2a\nuint64_empty=\nuint64_trailing=123xyz\n";

    INIReader reader(input.c_str(), input.size());
    Check("parse", reader.ParseError(), 0);

    errno = ERANGE;
    Check("long decimal", reader.GetInteger("numbers", "long_decimal", long_default), 42L);
    Check("long hex", reader.GetInteger("numbers", "long_hex", long_default), 42L);
    Check("long min", reader.GetInteger("numbers", "long_min", long_default), std::numeric_limits<long>::min());
    Check("long max", reader.GetInteger("numbers", "long_max", long_default), std::numeric_limits<long>::max());
    Check("long hex min", reader.GetInteger("numbers", "long_hex_min", long_default), std::numeric_limits<long>::min());
    Check("long hex max", reader.GetInteger("numbers", "long_hex_max", long_default), std::numeric_limits<long>::max());
    Check("long overflow", reader.GetInteger("numbers", "long_over", long_default), long_default);
    Check("long underflow", reader.GetInteger("numbers", "long_under", long_default), long_default);
    Check("long hex overflow", reader.GetInteger("numbers", "long_hex_over", long_default), long_default);
    Check("long hex underflow", reader.GetInteger("numbers", "long_hex_under", long_default), long_default);
    Check("long missing", reader.GetInteger("numbers", "long_missing", long_default), long_default);
    Check("long empty", reader.GetInteger("numbers", "long_empty", long_default), long_default);
    Check("long trailing", reader.GetInteger("numbers", "long_trailing", long_default), 123L);

    errno = ERANGE;
    Check("int64 decimal", reader.GetInteger64("numbers", "int64_decimal", int64_default), INT64_C(42));
    Check("int64 hex", reader.GetInteger64("numbers", "int64_hex", int64_default), INT64_C(42));
    Check("int64 min", reader.GetInteger64("numbers", "int64_min", int64_default), std::numeric_limits<int64_t>::min());
    Check("int64 max", reader.GetInteger64("numbers", "int64_max", int64_default), std::numeric_limits<int64_t>::max());
    Check("int64 hex min", reader.GetInteger64("numbers", "int64_hex_min", int64_default), std::numeric_limits<int64_t>::min());
    Check("int64 hex max", reader.GetInteger64("numbers", "int64_hex_max", int64_default), std::numeric_limits<int64_t>::max());
    Check("int64 overflow", reader.GetInteger64("numbers", "int64_over", int64_default), int64_default);
    Check("int64 underflow", reader.GetInteger64("numbers", "int64_under", int64_default), int64_default);
    Check("int64 hex overflow", reader.GetInteger64("numbers", "int64_hex_over", int64_default), int64_default);
    Check("int64 hex underflow", reader.GetInteger64("numbers", "int64_hex_under", int64_default), int64_default);
    Check("int64 missing", reader.GetInteger64("numbers", "int64_missing", int64_default), int64_default);
    Check("int64 empty", reader.GetInteger64("numbers", "int64_empty", int64_default), int64_default);
    Check("int64 trailing", reader.GetInteger64("numbers", "int64_trailing", int64_default), INT64_C(123));

    errno = ERANGE;
    Check("unsigned long decimal", reader.GetUnsigned("numbers", "ulong_decimal", ulong_default), 42UL);
    Check("unsigned long hex", reader.GetUnsigned("numbers", "ulong_hex", ulong_default), 42UL);
    Check("unsigned long zero", reader.GetUnsigned("numbers", "ulong_zero", ulong_default), 0UL);
    Check("unsigned long max", reader.GetUnsigned("numbers", "ulong_max", ulong_default), std::numeric_limits<unsigned long>::max());
    Check("unsigned long hex max", reader.GetUnsigned("numbers", "ulong_hex_max", ulong_default), std::numeric_limits<unsigned long>::max());
    Check("unsigned long overflow", reader.GetUnsigned("numbers", "ulong_over", ulong_default), ulong_default);
    Check("unsigned long hex overflow", reader.GetUnsigned("numbers", "ulong_hex_over", ulong_default), ulong_default);
    Check("unsigned long negative", reader.GetUnsigned("numbers", "ulong_negative", ulong_default), ulong_default);
    Check("unsigned long negative zero", reader.GetUnsigned("numbers", "ulong_negative_zero", ulong_default), ulong_default);
    Check("unsigned long missing", reader.GetUnsigned("numbers", "ulong_missing", ulong_default), ulong_default);
    Check("unsigned long empty", reader.GetUnsigned("numbers", "ulong_empty", ulong_default), ulong_default);
    Check("unsigned long trailing", reader.GetUnsigned("numbers", "ulong_trailing", ulong_default), 123UL);

    errno = ERANGE;
    Check("uint64 decimal", reader.GetUnsigned64("numbers", "uint64_decimal", uint64_default), UINT64_C(42));
    Check("uint64 hex", reader.GetUnsigned64("numbers", "uint64_hex", uint64_default), UINT64_C(42));
    Check("uint64 zero", reader.GetUnsigned64("numbers", "uint64_zero", uint64_default), UINT64_C(0));
    Check("uint64 max", reader.GetUnsigned64("numbers", "uint64_max", uint64_default), std::numeric_limits<uint64_t>::max());
    Check("uint64 hex max", reader.GetUnsigned64("numbers", "uint64_hex_max", uint64_default), std::numeric_limits<uint64_t>::max());
    Check("uint64 overflow", reader.GetUnsigned64("numbers", "uint64_over", uint64_default), uint64_default);
    Check("uint64 hex overflow", reader.GetUnsigned64("numbers", "uint64_hex_over", uint64_default), uint64_default);
    Check("uint64 negative", reader.GetUnsigned64("numbers", "uint64_negative", uint64_default), uint64_default);
    Check("uint64 negative zero", reader.GetUnsigned64("numbers", "uint64_negative_zero", uint64_default), uint64_default);
    Check("uint64 missing", reader.GetUnsigned64("numbers", "uint64_missing", uint64_default), uint64_default);
    Check("uint64 empty", reader.GetUnsigned64("numbers", "uint64_empty", uint64_default), uint64_default);
    Check("uint64 trailing", reader.GetUnsigned64("numbers", "uint64_trailing", uint64_default), UINT64_C(123));

    if (failures == 0)
        std::cout << "INIReader integer tests passed\n";
    return failures != 0;
}
