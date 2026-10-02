// Example that exercises section names containing '='.

#include <iostream>
#include "../cpp/INIReader.h"

static void PrintKeys(const INIReader& reader, const std::string& section)
{
    std::cout << "[" << section << "] exists=" << reader.HasSection(section) << ", keys:";
    std::vector<std::string> keys = reader.Keys(section);
    for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
        std::cout << " [" << *it << "]";
    }
    std::cout << "\n";
}

int main()
{
    const char config[] =
        "root=global\n"
        "[Foo=Bar]\n"
        "inner=one\n"
        "[FOO=bar]\n"
        "other=two\n"
        "[foo=bar=baz]\n"
        "leaf=three\n"
        "[Foo]\n"
        "zzz=real\n"
        "[=prefix]\n"
        "leading=yes\n"
        "[tail=]\n"
        "trailing=yes\n"
        "[Only=child]\n"
        "value=one\n"
        "[empty]\n";
    INIReader reader(config, sizeof(config) - 1);

    std::cout << "Parse error: " << reader.ParseError() << "\nSections:";
    std::vector<std::string> sections = reader.Sections();
    for (std::vector<std::string>::const_iterator it = sections.begin(); it != sections.end(); ++it) {
        std::cout << " [" << *it << "]";
    }
    std::cout << "\n";

    PrintKeys(reader, "");
    PrintKeys(reader, "foo");
    PrintKeys(reader, "FOO=BAR");
    PrintKeys(reader, "foo=bar=baz");
    PrintKeys(reader, "=prefix");
    PrintKeys(reader, "tail=");
    PrintKeys(reader, "tail");
    PrintKeys(reader, "only");
    PrintKeys(reader, "only=child");
    PrintKeys(reader, "empty");
    PrintKeys(reader, "missing");

    INIReader empty("", 0);
    std::cout << "Empty reader sections: " << empty.Sections().size() << "\n";
    PrintKeys(empty, "");
    return 0;
}
