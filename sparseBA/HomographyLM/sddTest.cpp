#include <boost/numeric/bindings/lapack/driver/gesdd.hpp>//svn
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
using namespace boost::numeric::bindings;
using namespace boost::numeric::ublas;
namespace ublas = boost::numeric::ublas;

typedef double real_t; 
typedef ublas::matrix<real_t, ublas::column_major> mat;
typedef ublas::vector<real_t> vec;

main()
{
   mat H(10,10), u(10,10), vt(10,10);
   vec s(10);
   lapack::gesdd( 'S', H, s, u, vt );
}
