#include <optional>

struct A
{
    int i;
};

void f(const A &a = A{})
{
}

class Class
{
public:
    struct Config
    {
        int i = 123;
    };
    void f(const Config &config) { }
    void f() { f(Config{}); }
};

int
main()
{
    std::optional<A> a_opt;
    a_opt.emplace(A{.i = 0});

    const A a{};

    return 0;
}
