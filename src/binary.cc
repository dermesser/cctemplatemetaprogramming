#include <iostream>

template<unsigned long In>
struct binary {
    static const unsigned long value = binary<In/10>::value * 2 + In%10;
};

template<>
struct binary<1> {
    static const unsigned value = 1;
};

int main(void) {
    std::cout << binary<1101011101>::value << "\n";
    return 0;
}
