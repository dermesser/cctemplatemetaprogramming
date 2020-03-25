#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <iostream>
#include <sstream>

namespace mpl = boost::mpl;

template <typename A1, typename A2>
struct plus {
    typedef typename mpl::integral_c<decltype(A1::value + A2::value),
                                     A1::value + A2::value>::type type;
};

template <typename A1, typename A2>
struct minus {
    typedef typename mpl::integral_c<
        typename std::make_signed<decltype(A1::value - A2::value)>::type,
        A1::value - A2::value>::type type;
};

/// `quote`
template <template <typename A1, typename... Rest> typename Op>
struct make_lambda {
    template <typename A1, typename... Rest>
    struct apply {
        typedef typename Op<A1, Rest...>::type type;
    };
};

// Unit definitions
typedef mpl::vector_c<int, 1, 0, 0, 0, 0, 0, 0> uM;
typedef mpl::vector_c<int, 0, 1, 0, 0, 0, 0, 0> uKg;
typedef mpl::vector_c<int, 0, 0, 1, 0, 0, 0, 0> uS;
typedef mpl::vector_c<int, 0, 0, 0, 1, 0, 0, 0> uA;
typedef mpl::vector_c<int, 0, 0, 0, 0, 1, 0, 0> uCd;
typedef mpl::vector_c<int, 0, 0, 0, 0, 0, 1, 0> uSrad;
typedef mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 1> uK;

template <typename U>
struct unit_name {
    std::string name(void) {
        std::ostringstream s;
#define EXTRACT_EXPONENT(n) int U##n = mpl::at<U, mpl::int_<n>>::type::value;
        EXTRACT_EXPONENT(0);
        EXTRACT_EXPONENT(1);
        EXTRACT_EXPONENT(2);
        EXTRACT_EXPONENT(3);
        EXTRACT_EXPONENT(4);
        EXTRACT_EXPONENT(5);
        EXTRACT_EXPONENT(6);
#define FORMAT_UNIT(USYM, UNAME)                \
    if (USYM) {                                 \
        s << #UNAME;                            \
        if (USYM != 1) s << "^" << USYM << " "; \
    }
        FORMAT_UNIT(U0, m);
        FORMAT_UNIT(U1, kg);
        FORMAT_UNIT(U2, s);
        FORMAT_UNIT(U3, A);
        FORMAT_UNIT(U4, cd);
        FORMAT_UNIT(U5, srad);
        FORMAT_UNIT(U6, K);
        return s.str();
    }
};

template <typename U1, typename U2>
struct multiply_units {
    typedef typename mpl::transform<U1, U2, make_lambda<plus>>::type type;
};

template <typename U1, typename U2>
struct divide_units {
    typedef typename mpl::transform<U1, U2, make_lambda<minus>>::type type;
};

// Unit prototypes contain a unit and know their name.

template <typename U>
struct unit_proto {
    typedef U type;
    std::string sym;
    unit_proto(void) : sym(unit_name<U>().name()) {}
    unit_proto(const std::string& sym) : sym(sym) {}
};

static const unit_proto<uM> meter;
static const unit_proto<uKg> kilogram;
static const unit_proto<uS> second;
static const unit_proto<uA> ampere;
static const unit_proto<uCd> candela;
static const unit_proto<uSrad> sterad;
static const unit_proto<uK> kelvin;

// Quantity

template <typename N, typename U>
struct quantity {
    N value;
    unit_proto<U> unit;

    // quantity(const quantity<N,U>& other) : value(other.value),
    // unit(other.unit) {} const quantity<N,U>& operator=(const quantity<N,U>&
    // other) { value = other.value; unit = other.unit; }
    quantity(N val, unit_proto<U> un) : value(val), unit(un) {}
    operator std::string(void) {
        std::ostringstream o;
        o << value << " " << unit_name<U>().name();
        return o.str();
    }

    template <typename N2>
    quantity<typename std::common_type<N, N2>::type, U> operator+(
        const quantity<N2, U>& other) {
        auto q = quantity<typename std::common_type<N, N2>::type, U>(
            value + other.value, unit);
        return q;
    }

    template <typename N2, typename U2>
    quantity<typename std::common_type<N, N2>::type,
             typename multiply_units<U, U2>::type>
    operator*(const quantity<N2, U2>& other) {
        typedef typename multiply_units<U, U2>::type resultunit;
        auto q = quantity<typename std::common_type<N, N2>::type, resultunit>(
            value * other.value, unit_proto<resultunit>());
        return q;
    }

    template <typename N2>
    quantity<typename std::common_type<N, N2>::type, U> operator*(
        const N2& other) {
        auto q = quantity<typename std::common_type<N, N2>::type, U>(
            value * other, unit_proto<U>());
        return q;
    }

    template <typename N2, typename U2>
    quantity<typename std::common_type<N, N2>::type,
             typename divide_units<U, U2>::type>
    operator/(const quantity<N2, U2>& other) {
        typedef typename divide_units<U, U2>::type resultunit;
        auto q = quantity<typename std::common_type<N, N2>::type, resultunit>(
            value / other.value, unit_proto<resultunit>());
        return q;
    }
};

template <typename N, typename U>
quantity<N, U> make_quant(N val, unit_proto<U> unit_pt) {
    quantity<N, U> q(val, unit_pt);
    return q;
}

int main(void) {
    auto x = make_quant(33.0f, second);
    auto y = make_quant(30.0f, meter);
    auto z = (x * x) / (x * y * 4.5);
    std::cout << std::string(z) << std::endl;
    return 0;
}

void test(void) {
    typedef mpl::vector_c<int, 1, 2, 3> a;
    typedef mpl::vector_c<int, 2, 3, 4> b;
    typedef make_lambda<mpl::plus> plus;

    typedef mpl::transform<a, b, plus>::type result;
    typedef mpl::at<result, mpl::int_<2>>::type first;
    std::cout << first::value << std::endl;
}

