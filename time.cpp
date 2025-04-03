#include <ctime>
#include <iostream>

int main() {
    std::tm tm{};
    tm.tm_year = 70;  // 1970 = 1900 + 70
    tm.tm_mon = 0;    // January = 0
    tm.tm_mday = 1;   // Day of month
    tm.tm_hour = 1;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    std::time_t t = std::mktime(&tm);
    std::cout << "time_t: " << t << "\n";
}
