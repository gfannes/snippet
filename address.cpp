#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

struct My
{
    const char *descr{};
    My(const char *descr)
        : descr(descr) {}

    void f(unsigned int level)
    {
#if 0
        int stack = level;
        std::cout << descr << ' ' << &stack << '\t' << this << std::endl;
        if (level > 0)
            f(level - 1);
#else
        const auto n = 1024*1024*1024;
        auto a = new char[n];
        auto b = new char[n];
        std::cout << (void *)a << ' ' << (void *)b << std::endl;
        delete[] b;
        delete[] a;
#endif
    }
};

struct Thread
{
    Thread()
        : thread_(std::ref(*this)) {}
    ~Thread()
    {
        quit_ = true;
        thread_.join();
    }

    void operator()()
    {
        std::ostringstream oss;
        oss << "thread-" << std::this_thread::get_id();
        const auto s = oss.str();

        auto my = new My(s.c_str());
        std::cout << "my: " << (void *)my << std::endl;
        my->f(1);
        delete my;

        while (!quit_)
        {
        }
        std::cout << "Thread " << s << " is stopping" << std::endl;
    }

    std::atomic_bool quit_ = false;
    std::thread thread_;
};

extern "C" {
    typedef struct
    {
        char data[3];
    } Code;
}

void f(Code code)
{
    std::cout << (void *)code.data << std::endl;
}

int main()
{
    {
        My my("stack");
        my.f(10);
    }

    auto my = new My("heap");
    my->f(10);
    delete my;

    Code code{};
    std::cout << (void *)code.data << std::endl;
    f(code);

    std::vector<Thread> threads(2);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
