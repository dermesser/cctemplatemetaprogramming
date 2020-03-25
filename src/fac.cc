#include <iostream>

template<unsigned N>
struct Fac {
    static const unsigned value = N * Fac<N-1>::value;
};

template<>
struct Fac<0> {
    static const unsigned value = 1;
};

int main(void) {

    std::cout << "3! = " << Fac<3>::value << std::endl;
    std::cout << "10! = " << Fac<10>::value << std::endl;
    return 0;
}
