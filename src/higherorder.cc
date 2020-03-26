#include <iostream>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/plus.hpp>

namespace mpl = boost::mpl;

struct plus10 {
    template<typename X>
    struct apply {
        typedef typename mpl::int_<10 + X::value>::type type;
    };
};

template<typename F, typename X>
struct apply_twice {
    typedef typename F::template apply<X>::type once;
    typedef typename F::template apply<once>::type type;
};

int main(void) {
    typedef mpl::int_<3> three;
    typedef mpl::lambda<mpl::plus<mpl::int_<11>, mpl::_1>>::type plus11;

    std::cout << apply_twice<plus11, three>::type::value << std::endl;
    return 0;
}
