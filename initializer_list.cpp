#include <initializer_list>

#include <iostream>

int main()
{
    auto lambda = [](std::initializer_list<int> il) {
        std::cout << "size: " << il.size() << std::endl;
        for (int v : il)
            std::cout << v << std::endl;
    };

    lambda({});
    lambda({1, 2, 3});
    return 0;
}
