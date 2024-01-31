#include <type_traits>
#include <iostream>

struct A
{
};

struct B : A
{
};

int main()
{
    std::cout << std::is_same_v<const A *, const B *> << std::endl;
    return 0;
}
