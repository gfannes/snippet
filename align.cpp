#include <iostream>
#include <vector>

int main()
{
    std::cout << sizeof(std::max_align_t) << std::endl;
    std::vector<std::max_align_t> buffer;

    for (auto size : {126, 127, 128, 129})
    {
        buffer.resize((size + sizeof(std::max_align_t) - 1) / sizeof(std::max_align_t));
        std::cout << size << " " << buffer.size() << " " << buffer.size() * sizeof(std::max_align_t) << std::endl;
    }
    return 0;
}
