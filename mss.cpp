#include <util/mss.hpp>

#include <iostream>

bool f()
{
    MSS_BEGIN(true);
    MSS(false);
    MSS_END();
}

std::optional<float> g()
{
    MSS_BEGIN(std::optional<float>{});
    MSS(std::make_optional(1.2f));
    MSS(false);
    MSS_END();
}

int main()
{
    mss::on_failure([](auto expr, auto file, auto line, auto function) {
        std::cout << "MSS error in " << function << " in '" << file << ':' << line << "': " << expr.type().name() << std::endl;
    });

    MSS_BEGIN(0);
    std::cout << f() << std::endl;
    auto opt = g();
    std::cout << opt.value_or(999) << std::endl;
    MSS_END();
}
