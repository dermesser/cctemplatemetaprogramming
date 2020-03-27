#include <iostream>
#include <boost/static_assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>

namespace mpl = boost::mpl;

template<typename A, typename B>
struct equal {
    typedef mpl::bool_<false> type;
};
template<typename A>
struct equal<A, A> {
    typedef mpl::bool_<true> type;
};

template<int N>
struct arg {
    template<typename A, typename ... As>
    struct apply : arg<N-1>::template apply<As...> {
    };
};

template<>
struct arg<1> {
    template<typename A, typename ... As>
    struct apply {
        typedef A type;
    };
};

struct plus10 {
    template<typename X>
    struct apply {
        typedef typename mpl::int_<10 + X::value>::type type;
    };
};

template<typename F, typename X>
struct apply : public F::template apply<X> {};

template<typename F, typename X>
struct apply_twice : public F::template apply<typename F::template apply<X>::type> {};

int exercises(void) {
    // 3-1
    {
        typedef mpl::vector_c<int, 1, 2, 3> src;
        typedef mpl::transform<src, mpl::plus<mpl::int_<1>, mpl::_>>::type dst;
        BOOST_STATIC_ASSERT(3 == mpl::at<dst, mpl::int_<1>>::type::value);
    }
    // 3-3
    {
        typedef mpl::lambda<apply_twice<mpl::lambda<std::add_pointer<mpl::_>>::type, mpl::_>, mpl::_>::type ptrtwice;
        typedef mpl::lambda<apply_twice<ptrtwice, mpl::_>>::type ptrquad;
        BOOST_STATIC_ASSERT(equal<double****, mpl::apply<ptrquad, double>::type>::type::value);
        std::cout << typeid(mpl::apply<ptrquad, double>::type).name();
    }
    return 0;
}

int arg_test(void) {
    BOOST_STATIC_ASSERT(3 == arg<2>::template apply<mpl::int_<1>, mpl::int_<3>, mpl::int_<2>>::type::value);
    std::cout << arg<2>::template apply<mpl::int_<1>, mpl::int_<3>, mpl::int_<2>>::type::value;
    return 0;
}

int test(void) {
    typedef mpl::int_<3> three;
    // lambda<> takes a placeholder expression and returns a metafunction class.
    typedef mpl::lambda<mpl::plus<mpl::int_<11>, mpl::_1>>::type plus11;
    typedef mpl::lambda<plus10>::type plus10_2;

    typedef mpl::vector_c<int, 1, 2, 3> seq1;
    typedef mpl::transform<seq1, plus10>::type seq2;
    std::cout << apply_twice<plus11, three>::type::value << " " << mpl::at<seq2, mpl::int_<2>>::type::value << std::endl;
    return 0;
}

int main(void) {
    exercises();
    return 0;
}

