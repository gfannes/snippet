#include <algorithm>
#include <iostream>

struct A
{
    int i = 0;

    void swap(A &other)// noexcept
    {
        std::cout << "my swap" << std::endl;
        auto tmp = i;
        i = other.i;
        other.i = tmp;
    }
};

namespace std {
    template<>
    void swap<A>(A &a, A &b) noexcept
    {
        a.swap(b);
    }
} // namespace std

int main()
{
    A a0{0};
    A a1{1};

    std::cout << a0.i << ' ' << a1.i << std::endl;
    std::swap(a0, a1);
    std::cout << a0.i << ' ' << a1.i << std::endl;

    return 0;
}
