#include <iostream>

extern "C" {
    typedef struct
    {
        char data[3];
    } Code;
}

void f(Code code)
{
    std::cout << (void *)&code << ' ' << code.data[0] << code.data[1] << code.data[2] << std::endl;
}

int main()
{
    Code code{};
    code.data[0] = 'a';
    code.data[1] = 'b';
    code.data[2] = 'c';
    std::cout << (void *)&code << ' ' << code.data[0] << code.data[1] << code.data[2] << std::endl;

    f(code);

    return 0;
}
