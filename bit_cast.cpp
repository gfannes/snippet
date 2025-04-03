#include <bit>
#include <cstdint>
#include <iostream>

int main()
{
    std::cout << "std::bit_cast" << std::endl;

    std::int32_t i32{};
    std::cout << std::bit_cast<std::uint32_t>(i32) << std::endl;

    std::int16_t i16{};
    std::cout << std::bit_cast<std::uint16_t>(i16) << std::endl;

    i16 = 0x8000;
    i32 = i16;
    std::cout << i16 << ' ' << i32 << std::endl;

    i32 <<= 8;
    std::cout << i16 << ' ' << i32 << std::endl;

    i32 >>= 8;
    std::cout << i16 << ' ' << i32 << std::endl;

    for (std::int32_t i4 = -8; i4 < 8; ++i4)
    {
        std::int32_t i2 = i4/4;
        std::int32_t i4_ = i2*4;
        std::cout << i4 << ' ' << i2 << ' ' << i4_ << std::endl;
    }

    for (std::int32_t i4 = -8; i4 < 8; ++i4)
    {
        std::int32_t i2 = i4>>2;
        std::int32_t i4_ = i2<<2;
        std::cout << i4 << ' ' << i2 << ' ' << i4_ << std::endl;
    }

    return 0;
}
