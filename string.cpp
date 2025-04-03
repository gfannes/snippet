#include <iostream>
#include <string>

int main()
{
    std::string str;
    str.resize(10, 'a');
    std::cout << str.size() << std::endl;
    str.resize_and_overwrite(15, [](auto ptr, auto size){std::cout << ptr << ' ' << size << std::endl; return 5;});
    std::cout << str.size() << ' ' << str.capacity() << std::endl;
    return 0;
}
