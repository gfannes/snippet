#include <cmath>
#include <fstream>
#include <iostream>
#include <numbers>

namespace my {
    const double pi = std::numbers::pi_v<double>;

    double atan_so(double x)
    {
        const auto b = 0.596227;
        return 0.5 * pi * (b * x + x * x) / (1 + 2 * b * x + x * x);
    }

    double atan_mtk(double x)
    {
        static const double b = pi / 6;
        static const double k = std::tan(b);
        static const double b0 = pi / 12;
        static const double k0 = std::tan(b0);

        static const double A = 0.999999020228907;
        static const double B = 0.257977658811405;
        static const double C = 0.59120450521312;

        double ang, x2;
        int comp = 0;
        int hi_seg = 0;

        int sign = (x < 0);
        x = std::abs(x);

        if (x > 1)
        {
            comp = 1;
            x = 1 / x;
        }

        if (x > k0)
        {
            hi_seg = 1;
            x = (x - k) / (1 + k * x);
        }

        x2 = x * x;
        ang = x * (A + B * x2) / (1 + C * x2);

        if (hi_seg)
            ang += b;

        if (comp)
            ang = pi / 2 - ang;

        return (sign ? -ang : ang);
    }
} // namespace my

int main()
{
    const auto data_fn = "atan.ssv";
    std::ofstream data_of{data_fn};

    const auto ep = 3.0;
    const auto eps = 0.01;
    for (double x = 0; x <= ep; x += eps)
    {
        const auto atan = std::atan(x);
        const auto atan_so = my::atan_so(x);
        const auto atan_mtk = my::atan_mtk(x);
        data_of << x << ' ' << atan << ' ' << atan_so << ' ' << atan_mtk << ' ' << atan_so - atan << ' ' << atan_mtk - atan << std::endl;
    }

    const auto gnuplot_fn = "atan.gnuplot";
    std::ofstream gnuplot_of{gnuplot_fn};
    gnuplot_of << "plot '" << data_fn << "' using 1:2 with lines title 'std::atan', '' using 1:3 with lines title 'atan_so', '' using 1:4 with lines title 'atan_mtk'" << std::endl;
    gnuplot_of << "pause -1" << std::endl;
    gnuplot_of << "plot '" << data_fn << "' using 1:6 with lines title 'so-std', '' using 1:6 with lines title 'mtk-std'" << std::endl;
    gnuplot_of << "pause -1" << std::endl;

    return 0;
}
