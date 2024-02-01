#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>

std::map<std::string, std::string> tt;

template<typename A, typename B>
void print()
{
    A a{};
    B b{};
    auto c = a + b;
    std::cout << tt[typeid(a).name()] << " + " << tt[typeid(b).name()] << " = " << tt[typeid(c).name()] << std::endl;
}

int main()
{
    std::cout << "bits: " << sizeof(void *) * 8 << std::endl;

#define print_type(type)                                                                               \
    std::cout << typeid(type).name() << ": " << #type << " (" << sizeof(type) * 8 << ")" << std::endl; \
    tt[typeid(type).name()] = #type

    using i = int;
    using u = unsigned int;
    using ii = long;
    using uu = unsigned long;
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;
    using s = std::size_t;

    print_type(i);
    print_type(u);
    print_type(ii);
    print_type(uu);
    print_type(s);
    print_type(i32);
    print_type(u32);
    print_type(i64);
    print_type(u64);

    std::cout << std::endl;

    print<i, i>();
    print<i, u>();
    print<u, i>();
    print<u, u>();

    std::cout << std::endl;

    print<i32, i64>();
    print<u32, u64>();
    print<i32, u64>();
    print<u32, i64>();

    std::cout << std::endl;

    print<s, i64>();

    return 0;
}
