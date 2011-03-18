#include<fundamentalMatrix.h>
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/lapack/driver/gesdd.hpp>//svn
#include <boost/numeric/ublas/io.hpp>
#include "utils.h"//for prints
using namespace boost::numeric::bindings;
using namespace boost::numeric::ublas;
using std::size_t; 
using std::cout;
using std::endl;
using std::string;
using std::ifstream; 
namespace ublas = boost::numeric::ublas;

void fundamentalMatrix::leastSqBlock(mrange &f, mrow &d) 
{
   assert(f.size1() == nIndepEq && f.size2() == nParams);
   real_t x = d(0), y=d(1), xp=d(2), yp=d(3);
   f(0,0) = xp*x;
   f(0,1) = xp*y;
   f(0,2) = xp;
   f(0,3) = yp*x;
   f(0,4) = yp*y;
   f(0,5) = yp;
   f(0,6) = x;
   f(0,7) = y;
   f(0,8) = 1;
}

real_t fundamentalMatrix::computeError(const mat &f, const mrow &data) 
{
   assert(data.size() == 4 && f.size1() == 3 && f.size2() == 3);
   vec x1(3),x2(3);
   x1(0)=data(0);x1(1)=data(1);x1(2)=1;
   x2(0)=data(2);x2(1)=data(3);x2(2)=1;
   
   return fabs(inner_prod(prod (x2, f),x1));//x2 F x1
}
