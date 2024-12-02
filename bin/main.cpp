
#include <lib/number.h>
#include <iostream>

int main() {
    int2023_t value = from_string("0");
    int2023_t result = from_string("2");
    int2023_t x = from_string("-1025");
    int2023_t y = from_string("1023");
    std::cout << (result * y) << std::endl;
    std::cout << '\n';
    system("pause");
    return 0;
}
