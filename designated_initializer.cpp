#include <optional>

struct A
{
    int i;
};

int
main()
{
    std::optional<A> a_opt;
    a_opt.emplace(A{.i = 0});
    return 0;
}
