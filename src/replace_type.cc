#include <iostream>

class C {};

template<typename C, typename X, typename Y>
struct replace_type {
    typedef C type;
};

template<typename X, typename Y>
struct replace_type<X*, X, Y> {
    typedef Y* type;
};

template<typename X, typename Y>
struct replace_type<X&, X, Y> {
    typedef Y& type;
};

template<typename R, typename X, typename Y>
struct replace_type<R (*)(X), X, Y> {
    typedef R (*type)(Y);
};

template<typename X, typename Y>
struct replace_type<X (*)(X), X, Y> {
    typedef Y (*type)(Y);
};

template<typename A, typename X, typename Y>
struct replace_type<X (*)(A), X, Y> {
    typedef Y (*type)(A);
};

int main(void) {
    typedef char& (*typ)(char& a);
    std::cout << typeid(typ).name() << std::endl;
    std::cout << typeid(replace_type<typ, unsigned, unsigned>::type).name() << std::endl;
    return 0;
}
