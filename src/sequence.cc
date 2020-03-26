#include <iostream>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/list_c.hpp>
#include <boost/static_assert.hpp>

namespace mpl = boost::mpl;

typedef mpl::vector_c<int, 1, 2, 3> seq1;
typedef mpl::list_c<int, 1, 2, 3> seq2;

BOOST_STATIC_ASSERT(mpl::equal<seq1, seq2>::value);

int main(void) {

    return 0;
}
