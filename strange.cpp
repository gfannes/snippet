#include <util/mss.hpp>

#include <iostream>
#include <span>

using Str = std::span<const char>;

namespace utf8 {

    char32_t from_1b(Str str)
    {
        return str.front();
    }
    char32_t from_2b(Str str)
    {
        const auto a = (std::uint32_t)str[0] & 0x3f;
        const auto b = (std::uint32_t)str[1] & 0x7f;
        return (a << 7) | b;
    }
    char32_t from_3b(Str str)
    {
        const auto a = (std::uint32_t)str[0] & 0x1f;
        const auto b = (std::uint32_t)str[1] & 0x7f;
        const auto c = (std::uint32_t)str[2] & 0x7f;
        return (a << 14) | (b << 7) | c;
    }
    char32_t from_4b(Str str)
    {
        const auto a = (std::uint32_t)str[0] & 0x0f;
        const auto b = (std::uint32_t)str[1] & 0x7f;
        const auto c = (std::uint32_t)str[2] & 0x7f;
        const auto d = (std::uint32_t)str[3] & 0x7f;
        return (a << 21) | (b << 14) | (c << 7) | d;
    }

    std::optional<std::pair<char32_t, Str>> split_first(Str str)
    {
        if (!str.empty())
        {
            const auto byte = (std::uint8_t)str.front();
            if ((byte & 0x80) == 0x00)
                return std::make_pair(from_1b(str), str.subspan<1>());
            if ((byte & 0xe0) == 0xc0 && str.size() >= 2)
                return std::make_pair(from_2b(str), str.subspan<2>());
            if ((byte & 0xf0) == 0xe0 && str.size() >= 3)
                return std::make_pair(from_3b(str), str.subspan<3>());
            if ((byte & 0xf8) == 0xf0 && str.size() >= 4)
                return std::make_pair(from_4b(str), str.subspan<4>());
        }
        return {};
    }
} // namespace utf8

class Strange
{
public:
    Strange(const std::string &string)
        : str_(string.data(), string.size()) {}

    std::size_t size() const { return str_.size(); }

    bool pop_if(char32_t wanted)
    {
        MSS_BEGIN(true);
        const auto opt = utf8::split_first(str_);
        MSS_Q(!!opt && opt->first == wanted);
        str_ = opt->second;
        MSS_END();
    }

private:
    Str str_;
};

int main()
{
    MSS_BEGIN(0);

    const std::size_t size = 1024 * 1024 * 1024;
    std::string content(size, 'a');
    std::cout << size << std::endl;

    Strange strange{content};
    MSS(strange.size() == size);

    while (strange.pop_if('a'))
    {
    }

    MSS_END();
}
