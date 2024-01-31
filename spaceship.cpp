#include <compare>
#include <optional>
#include <variant>

struct A
{
    int i = 0;

    // auto operator<=>(const A &) const = default;
    auto operator<=>(const A &a) const
    {
        return std::strong_ordering::equal;
    }
    // When implementing operator<=>(), one also needs to implement operator==()
    auto operator==(const A &a) const
    {
        return true;
    }
};

struct B
{
    // A a;
    std::variant<A> a;

    auto operator<=>(const B &) const = default;
};

int main()
{
    return 0;
}
