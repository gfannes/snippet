#include <string>
#include <iostream>

int main()
{
    const std::string str = "abc";
    const std::string_view sv = str;
    std::cout << sizeof(sv) << std::endl;
    return 0;
}
