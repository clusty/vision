#include "optimFunction.hpp"
template<typename T>
class rosenbrock:public optimFunction<T>{
   private:
      template<typename G>
         inline G evalFDummy(std::vector<G> &x)
         {
            G sum=0;
	         for (int i=0;i<x.size()-1;i++)
               sum=sum+pow(1-x[i],2)+100*pow(x[i+1]-x[i]*x[i],2);
	         return sum;

         }
   public:   virtual T evalF(std::vector<T>&x)
             {
                return evalFDummy(x);
             }

             virtual CppAD::AD<T> evalF(std::vector<CppAD::AD< T> >&x)
             {
                return evalFDummy(x);
             }

             rosenbrock(int ndim):
                optimFunction<T>(ndim){}
};
