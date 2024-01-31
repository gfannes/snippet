#include <iostream>

template<typename T>
struct A;

template<>
struct A<int>
{
    static constexpr const char *name = "int";
};

int main()
{
    std::cout << A<int>::name << std::endl;
    return 0;
}
