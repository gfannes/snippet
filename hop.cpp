#include <iostream>
#include <numeric>
#include <vector>

template<unsigned int N, typename It>
class Hop
{
public:
    using difference_type = typename It::difference_type;
    using value_type = typename It::value_type;
    using pointer = typename It::pointer;
    using reference = typename It::reference;
    using iterator_category = typename It::iterator_category;

    Hop() {}
    Hop(It it)
        : it_(it) {}

    auto operator*() -> reference { return *it_; }
    auto operator++() { std::advance(it_, N); }
    auto operator-(const Hop &rhs) const -> difference_type { return (it_ - rhs.it_) / N; }

    auto operator==(const Hop &rhs) const -> bool { return it_ == rhs.it_; }
    auto operator!=(const Hop &rhs) const -> bool { return it_ != rhs.it_; }

private:
    It it_{};
};

template<unsigned int N, typename It>
auto make_hop(It it) -> Hop<N, It>
{
    return Hop<N, It>{it};
}

template<unsigned int N, typename Container>
auto make_hop_pair(const Container &container, std::size_t offset = 0)
{
    const auto begin = container.begin() + offset;
    auto count = std::distance(begin, container.end());
    if (count <= 0)
        return std::make_pair(make_hop<N>(begin), make_hop<N>(begin));

    count = ((count + N - 1) / N) * N;
    return std::make_pair(make_hop<N>(begin), make_hop<N>(begin + count));
}

template<unsigned int N, typename Container>
auto make_hop_pair(Container &container, std::size_t offset = 0)
{
    const auto begin = container.begin() + offset;
    auto count = std::distance(begin, container.end());
    if (count <= 0)
        return std::make_pair(make_hop<N>(begin), make_hop<N>(begin));

    count = ((count + N - 1) / N) * N;
    return std::make_pair(make_hop<N>(begin), make_hop<N>(begin + count));
}

int main()
{
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 0);

    if (false)
    {
        auto hop = make_hop<2>(v.begin());
        for (auto i = 0; i < 5; ++i)
        {
            std::cout << *hop << std::endl;
            ++hop;
        }
    }

    {
        auto p = make_hop_pair<3>(v, 2);
        std::cout << "distance:" << std::distance(p.first, p.second) << std::endl;
        for (auto it = p.first; it != p.second; ++it)
        {
            std::cout << *it << std::endl;
        }

        std::iota(p.first, p.second, 100);
        std::cout << std::endl;
        for (auto v : v)
            std::cout << v << std::endl;
    }

    return 0;
}
