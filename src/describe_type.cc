#include <iostream>
#include <string>

using std::string;

template<typename T>
struct describe {
    static const string value;
    static const string rest;
};

template<typename T>
const string describe<T>::value = "???";
template<typename T>
const string describe<T>::rest = "";

// Basic types
#define BASIC_TYPE_DESC(TYP) template<> \
struct describe<TYP> { \
    static const string value; \
    static const string rest; \
}; \
const string describe<TYP>::value = #TYP; \
const string describe<TYP>::rest = "";

BASIC_TYPE_DESC(char)
BASIC_TYPE_DESC(short)
BASIC_TYPE_DESC(int)
BASIC_TYPE_DESC(unsigned)
BASIC_TYPE_DESC(long)
BASIC_TYPE_DESC(unsigned long)
BASIC_TYPE_DESC(long long)
BASIC_TYPE_DESC(unsigned long long)
BASIC_TYPE_DESC(float)
BASIC_TYPE_DESC(double)

#define COMPOUND_REFERENCE_DECL(REF) \
template<typename T> \
struct describe<T REF> { \
    static const string value; \
    static const string rest; \
}; \
template<typename T> \
const string describe<T REF>::value = #REF; \
template<typename T> \
const string describe<T REF>::rest = describe<T>::rest+describe<T>::value;

COMPOUND_REFERENCE_DECL(*);
COMPOUND_REFERENCE_DECL(* const);
COMPOUND_REFERENCE_DECL(&);
COMPOUND_REFERENCE_DECL(&&);
COMPOUND_REFERENCE_DECL([]);

template<typename T>
struct describe<const T> {
    static const string value;
    static const string rest;
};
template<typename T>
const string describe<const T>::value = describe<T>::value;
template<typename T>
const string describe<const T>::rest = describe<T>::rest+"const ";

template<typename R, typename A1>
struct describe<R (*)(A1)> {
    static const string value;
    static const string rest;
};
template<typename R, typename A1>
const string describe<R(*)(A1)>::value = describe<R>::rest+describe<R>::value+"(*)("+describe<A1>::rest+describe<A1>::value+")";
template<typename R, typename A1>
const string describe<R(*)(A1)>::rest = "";

int main(void) {
    typedef long long typ[];
    std::cout << typeid(typ).name() << " <=> " << describe<typ>::rest+describe<typ>::value << "\n";
    return 0;
}
