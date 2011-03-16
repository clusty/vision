#include<vector>
#include<math.h>
#ifndef OPTIM_FUNCTION_H
#define OPTIM_FUNCTION_H

#define sigma0 0.0001
#define eps 1e-12

class optimFunction{
   public:   virtual double evalF(std::vector<double>&)=0;//need to implement
             virtual std::vector<double> evalGrad(std::vector<double>&,double =1e-6 );//FD gradient
             //start point, niter, stop is grad< 1e-12 or function change 1e-12
             int scaledConjugateGradient(std::vector<double>&,int=1000,double=1e-12,double=1e-12);


};

inline double dot(std::vector<double>&a,std::vector<double>&b)
{
	double res=0;
	for (int i=0;i<a.size();i++)
		res+=a[i]*b[i];
	return res;
}
//matrix vector product
inline std::vector<double> mv(std::vector<std::vector<double> > &m,std::vector<double> &v)
{
   std::vector<double> res(m.size());
   for (int i=0;i<m.size();i++)
      res[i]=dot(m[i],v);

   return res;
}

inline double norm (std::vector<double> &a)
{
   return sqrt(dot(a,a));
}


#endif
