#include <boost/mpl/at.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/equal.hpp>
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

template <typename A1>
struct negate {
    typedef typename mpl::integral_c<
        typename std::make_signed<decltype(A1::value)>::type, -A1::value>::type
        type;
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
typedef mpl::vector_c<int, 1, 0, 0, 0, 0, 0, 0> METER;
typedef mpl::vector_c<int, 0, 1, 0, 0, 0, 0, 0> KILOGRAM;
typedef mpl::vector_c<int, 0, 0, 1, 0, 0, 0, 0> SECOND;
typedef mpl::vector_c<int, 0, 0, 0, 1, 0, 0, 0> AMPERE;
typedef mpl::vector_c<int, 0, 0, 0, 0, 1, 0, 0> CANDELA;
typedef mpl::vector_c<int, 0, 0, 0, 0, 0, 1, 0> SRAD;
typedef mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 1> KELVIN;

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
#define FORMAT_UNIT(USYM, UNAME)         \
    if (USYM) {                          \
        s << #UNAME;                     \
        if (USYM != 1) s << "^" << USYM; \
        s << " ";                        \
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

template <typename U>
struct invert_units {
    typedef typename mpl::transform<U, make_lambda<negate>>::type type;
};

// Quantity

template <typename N, typename U, std::enable_if_t<std::is_arithmetic<N>::value, int> = 0>
struct quantity {
    N value;

    // quantity(const quantity<N,U>& other) : value(other.value),
    // unit(other.unit) {} const quantity<N,U>& operator=(const quantity<N,U>&
    // other) { value = other.value; unit = other.unit; }
    quantity(N val) : value(val) {}
    template<typename U2>
    quantity(const quantity<N, U2>& other) : value(other.value) {
        BOOST_STATIC_ASSERT(mpl::equal<U, U2>::type::value);
    }
    operator std::string(void) {
        std::ostringstream o;
        o << value << " " << unit_name<U>().name();
        return o.str();
    }

    template <typename N2>
    quantity<typename std::common_type<N, N2>::type, U> operator+(
        const quantity<N2, U>& other) const {
        auto q = quantity<typename std::common_type<N, N2>::type, U>(
            value + other.value);
        return q;
    }

    template <typename N2, typename U2>
    quantity<typename std::common_type<N, N2>::type,
             typename multiply_units<U, U2>::type>
    operator*(const quantity<N2, U2>& other) const {
        typedef typename multiply_units<U, U2>::type resultunit;
        auto q = quantity<typename std::common_type<N, N2>::type, resultunit>(
            value * other.value);
        return q;
    }

    template <typename N2, typename U2>
    quantity<typename std::common_type<N, N2>::type,
             typename divide_units<U, U2>::type>
    operator/(const quantity<N2, U2>& other) const {
        typedef typename divide_units<U, U2>::type resultunit;
        auto q = quantity<typename std::common_type<N, N2>::type, resultunit>(
            value / other.value);
        return q;
    }
};

// Scalar multiplication/division
template <typename N1, typename N2, typename U,
          typename std::enable_if<std::is_arithmetic<N1>::value, int>::type = 0>
quantity<typename std::common_type<N1, N2>::type, U> operator*(
    const quantity<N2, U>& second, const N1& first) {
    auto q = quantity<typename std::common_type<N1, N2>::type, U>(second.value *
                                                                  first);
    return q;
}
template <typename N1, typename N2, typename U,
          typename std::enable_if<std::is_arithmetic<N1>::value, int>::type = 0>
quantity<typename std::common_type<N1, N2>::type, U> operator*(
    const N1& first, const quantity<N2, U>& second) {
    return second * first;
}

template <typename N1, typename N2, typename U,
          typename std::enable_if<std::is_arithmetic<N1>::value, int>::type = 0>
quantity<typename std::common_type<N1, N2>::type, U> operator/(
    quantity<N1, U> first, N2 second) {
    auto q = quantity<typename std::common_type<N1, N2>::type, U>(first.value /
                                                                  second);
    return q;
}
template <typename N1, typename N2, typename U,
          typename std::enable_if<std::is_arithmetic<N1>::value, int>::type = 0>
quantity<typename std::common_type<N1, N2>::type,
         typename mpl::transform<U, make_lambda<negate>>::type>
operator/(N1 first, quantity<N2, U> second) {
    typedef typename mpl::transform<U, make_lambda<negate>>::type inverted;
    return quantity<typename std::common_type<N1, N2>::type, inverted>(
        first / second.value);
}

static const quantity<int, METER> meter(1);
static const quantity<int, SECOND> second(1);
static const quantity<int, KILOGRAM> kilogram(1);
static const quantity<int, AMPERE> ampere(1);
static const quantity<int, CANDELA> candela(1);
static const quantity<int, SRAD> srad(1);
static const quantity<int, KELVIN> kelvin(1);

static const quantity<int, divide_units<multiply_units<KILOGRAM, METER>::type, multiply_units<SECOND, SECOND>::type>::type> newton(1);

int main(void) {
    auto force = 1 * kilogram * meter / (second*second);
    std::cout << std::string(4 * newton + force) << std::endl;
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

