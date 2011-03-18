#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>
#include <fstream>
#include <utils.h>
class fundamentalMatrix{
   public:
      fundamentalMatrix() {};
      //fundamentalMatrix(const std::string &name);
      static const int nIndepEq = 1;//each pair gives 1 constraint
      static const int nParams = 9; //9 params (8 in fact)
      //one data point gives you nIndepEq equations
      //and we need are trying to fit nParams
      static void leastSqBlock(mrange &f, mrow &d) ;
      static real_t computeError(const mat &f, const mrow &data) ;

      
   private:
      //mat _data;
};

