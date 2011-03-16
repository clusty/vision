#include "optimFunction.h"

class rosenbrock:public optimFunction{
   public:   virtual double evalF(std::vector<double>&x)
             {
                double sum=0;
	             for (int i=0;i<x.size()-1;i++)
                   sum+=pow(1-x[i],2)+100*pow(x[i+1]-x[i]*x[i],2);
	             return sum;
             }
};
