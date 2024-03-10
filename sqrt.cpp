#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>

namespace my {
    unsigned int sqrt_slow(int x)
    {
        unsigned long rem = 0;
        unsigned long root = 0;
        unsigned long divisor = 0;

        for (auto i = 0u; i < 16; ++i)
        {
            root <<= 1;
            rem = ((rem << 2) + (x >> 30));
            x <<= 2;
            divisor = (root << 1) + 1;
            if (divisor <= rem)
            {
                rem -= divisor;
                ++root;
            }
        }
        return root;
    }
    unsigned int sqrt(int x)
    {
        unsigned long rem = 0;
        unsigned long root = 0;

        for (auto i = 0u; i < 16; ++i)
        {
            root <<= 1;
            rem = ((rem << 2) + (x >> 30));
            x <<= 2;
            ++root;
            if (root <= rem)
            {
                rem -= root;
                ++root;
            }
            else
                --root;
        }

        return (unsigned int)(root >> 1);
    }
    unsigned int isqrt(unsigned int x)
    {
        if (x <= 1) return x;

        unsigned int x1;
        int s, g0, g1;

        s = 1;
        x1 = x - 1;
        if (x1 > 65535) { s = s + 8; x1 = x1 >> 16; }
        if (x1 > 255) { s = s + 4; x1 = x1 >> 8; }
        if (x1 > 15) { s = s + 2; x1 = x1 >> 4; }
        if (x1 > 3) { s = s + 1; }

        g0 = 1 << s;
        g1 = (g0 + (x >> s)) >> 1;

        while (g1 < g0)
        {
            g0 = g1;
            g1 = (g0 + (x / g0)) >> 1;
        }

        return g0;
    }
    unsigned int isqrt2(int x)
    {
        if (x <= 1) return x;

        int guess = x;
        int e, temp;
        auto count = 0;
        do
        {
            temp = x / guess;
            e = (guess - temp) / 2;
            guess = (guess + temp) / 2;
            ++count;
        } while (e != 0);
        std::cout << count << std::endl;

        return guess;
    }
} // namespace my

int main()
{
    const double factor1 = std::pow(2.0, 23.0);
    const double factor2 = std::pow(2.0, 23.0 / 2);

    const auto data_fn = "sqrt.ssv";
    std::ofstream data_of{data_fn};

    const auto range = std::make_pair(0.0, 100.0);
    const auto d = 0.01;
    for (auto x = range.first; x <= range.second; x += d)
    {
        const auto std_sqrt = std::sqrt(x);
        const auto my_sqrt = my::sqrt(x * factor1) / factor2;
        const auto my_sqrt_slow = my::sqrt_slow(x * factor1) / factor2;
        const auto my_isqrt = my::isqrt(x * factor1) / factor2;
        const auto my_isqrt2 = my::isqrt2(x * factor1) / factor2;

        data_of << x << ' ' << std_sqrt << ' ' << my_sqrt << ' ' << my_sqrt_slow << ' ' << my_isqrt << ' ' << my_isqrt2 << ' ' << my_isqrt - std_sqrt + 0.00001 << ' ' << my_isqrt2 - std_sqrt << std::endl;
    }

    const auto gnuplot_fn = "sqrt.gnuplot";
    std::ofstream gnuplot_of{gnuplot_fn};
    gnuplot_of << "plot '" << data_fn << "' using 1:2 with lines title 'std::sqrt', '' using 1:3 with lines title 'my::sqrt', '' using 1:4 with lines title 'my::sqrt_slow', '' using 1:5 with lines title 'my::isqrt', '' using 1:6 with lines title 'my::isqrt2'" << std::endl;
    gnuplot_of << "pause -1" << std::endl;
    gnuplot_of << "plot '" << data_fn << "' using 1:7 with lines title 'diff', '' using 1:8 with lines title '2'" << std::endl;
    gnuplot_of << "pause -1" << std::endl;

    return 0;
}
