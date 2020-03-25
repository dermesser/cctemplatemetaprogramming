#include <iostream>
#include <type_traits>
#include <boost/mpl/if.hpp>

namespace mpl = boost::mpl;

template<typename C, typename X, typename Y>
struct replace_type {
    typedef C type;
};

// Simple replacement.
template<typename X, typename Y>
struct replace_type<X, X, Y> {
    typedef Y type;
};

namespace {
    // For pointer/reference types, if C* == X, replace, otherwise continue replacing.
    template<bool is_same, typename C, typename X, typename Y>
    struct replace_type_ptr_helper {
        typedef Y type;
    };
    template<typename C, typename X, typename Y>
    struct replace_type_ptr_helper<true, C*, X, Y> {
        typedef Y type;
    };
    template<typename C, typename X, typename Y>
    struct replace_type_ptr_helper<false, C*, X, Y> {
        typedef typename replace_type<C, X, Y>::type* type;
    };
    template<typename C, typename X, typename Y>
    struct replace_type_ptr_helper<true, C&, X, Y> {
        typedef Y type;
    };
    template<typename C, typename X, typename Y>
    struct replace_type_ptr_helper<false, C&, X, Y> {
        typedef typename replace_type<C, X, Y>::type& type;
    };
    template<typename C, typename X, typename Y>
    struct replace_type_ptr_helper<true, C&&, X, Y> {
        typedef Y type;
    };
    template<typename C, typename X, typename Y>
    struct replace_type_ptr_helper<false, C&&, X, Y> {
        typedef typename replace_type<C, X, Y>::type&& type;
    };
}

template<typename C, typename X, typename Y>
struct replace_type<C*, X, Y> {
    //typedef typename replace_type_ptr_helper<std::is_same<C*, X>::value, C*, X, Y>::type type;
    typedef typename mpl::if_<std::is_same<C*, X>, Y, typename replace_type<C, X, Y>::type*>::type type;
};

template<typename C, typename X, typename Y>
struct replace_type<C&, X, Y> {
    //typedef typename replace_type_ptr_helper<std::is_same<C&, X>::value, C&, X, Y>::type type;
    typedef typename mpl::if_<std::is_same<C&, X>, Y, typename replace_type<C, X, Y>::type&>::type type;
};

template<typename C, typename X, typename Y>
struct replace_type<C&&, X, Y> {
    //typedef typename replace_type_ptr_helper<std::is_same<C&&, X>::value, C&&, X, Y>::type type;
    typedef typename mpl::if_<std::is_same<C&&, X>, Y, typename replace_type<C, X, Y>::type&&>::type type;
};

template<typename C, typename X, typename Y>
struct replace_type<const C, X, Y> {
    //typedef const typename replace_type_ptr_helper<std::is_same<const C, X>::value, const C, X, Y>::type type;
    typedef typename mpl::if_<std::is_same<const C, X>, Y, const typename replace_type<C, X, Y>::type>::type type;
};

template<typename R, typename A, typename X, typename Y>
struct replace_type<R (*)(A), X, Y> {
    typedef typename replace_type<R, X, Y>::type (*type)(typename replace_type<A, X, Y>::type);
};

int main(void) {
    typedef const long long& (*typ)(long long&&);
    std::cout << typeid(typ).name() << std::endl;
    std::cout << typeid(replace_type<typ, long long, unsigned>::type).name() << std::endl;
    return 0;
}
