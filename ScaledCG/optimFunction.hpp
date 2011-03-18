#include<vector>
#include<math.h>
#include<iostream>
#ifndef OPTIM_FUNCTION_H
#define OPTIM_FUNCTION_H

#define sigma0 0.0001
#define eps 1e-12

#include <cppad/cppad.hpp> // the CppAD package http://www.coin-or.org/CppAD/
 using CppAD::AD;

template<typename T>
class optimFunction{
  public: 
   optimFunction(int n); 
private:
   optimFunction(){};
   int _dim;
   std::vector< CppAD::AD<T> > _X, _Y;
   CppAD::ADFun<T> _f;
public:  
   virtual T evalF(std::vector<T>&) =0;//need to implement
   virtual CppAD::AD<T> evalF(std::vector<CppAD::AD<T> >&) =0;//need to implement

   std::vector<T> evalGrad(std::vector<T>&,T =1e-6 );
   void initialize();
   //FD or AD gradient
   //start point, niter, stop is grad< 1e-12 or function change 1e-12
   int scaledConjugateGradient(std::vector<T>&,int =1000,T=1e-12,T=1e-12);
};
template<typename T>
inline T dot(std::vector<T>&a,std::vector<T>&b)
{
	T res=0;
	for (int i=0;i<a.size();i++)
		res+=a[i]*b[i];
	return res;
}
//matrix vector product
template<typename T>
inline std::vector<T> mv(std::vector<std::vector<T> > &m,std::vector<T> &v)
{
   std::vector<T> res(m.size());
   for (int i=0;i<m.size();i++)
      res[i]=dot(m[i],v);

   return res;
}
template<typename T>
inline T norm (std::vector<T> &a)
{
   return sqrt(dot(a,a));
}

template<typename T>
optimFunction<T>::optimFunction(int ndim):
   _dim(ndim),
   _X(ndim)
{
}

template<typename T>
void optimFunction<T>::initialize()
{
   _X.resize(_dim);
   _Y.resize(1);
   for (int i=0;i<_dim;i++)
      _X[i] = rand();
   
   CppAD::Independent(_X);
   _Y[0] = evalF(_X);
   CppAD::ADFun<T> f(_X, _Y);
   _f = f;
}


template<typename T>
std::vector<T> optimFunction<T>::evalGrad(std::vector<T>&x,T h)
{
   //compute finite difference gradient (central difference)
   /*std::vector<T>grad(x.size());
   std::vector<T> xh=x;
	for (int i=0;i<x.size();i++)
	{

      xh[i] += h;    
      T fxph = evalF(xh);
      xh[i] -= 2*h; // -h brings us back to original val and another -h at -h
      T fxmh = evalF(xh);
      grad[i] = (fxph - fxmh)/ (2*h);


      xh[i] +=h; //bring back component so we are at +0
	}
   return grad;*/
   return _f.Jacobian( x );
}

template<typename T>
int optimFunction<T>::scaledConjugateGradient(std::vector<T>&x,int niter,T gradNorm,
      T fDecrease)//stop when ALL gradient is too small, function decrease is small
            //or did all the iterations

{
	int nsuccess=0,j;
	T h=1e-7;
   std::vector<T> gradnew,gradold,d,xplus,gplus,xnew;
	T fnew,fold,fnow,beta,betamin,betamax,gamma;
	T mu,kappa,sigma,theta,delta,alpha;
	T Delta;
	bool success;
	////
	d.resize(x.size());
	xplus.resize(x.size());
	xnew.resize(x.size());
   fold=evalF(x);
	fnow=fold;
   gradnew=evalGrad(x);
	gradold=gradnew;
	for (int i=0;i<d.size();i++)
		d[i]=-gradnew[i];
	success=true;
	beta=1;
	betamin = 1.0e-15; 
	betamax = 1.0e100;
	j=1;
	while (j<=niter)
	{
		if (success==true)
		{
			mu= dot(d,gradnew);//dot product
		    if (mu >= 0)
			{
				for (int i=0;i<d.size();i++)
					d[i]=-gradnew[i];
				
				mu = dot(d,gradnew);
			}
			kappa=dot(d,d);
			if (kappa<eps)
			{
            std::cerr << "small gradient:"<<"converged in "<<j
               <<" iterations with F="<<fnew<<std::endl;
				return 0;//gradient norm smaller than eps
			}
			sigma=sigma0/sqrt(kappa);
			for (int i=0;i<xplus.size();i++)
				xplus[i] = x[i] + sigma*d[i];
         gplus=evalGrad(xplus);
         std::vector<T> tmp;
			tmp.resize(gradnew.size());
			for (int i=0;i<gradnew.size();i++)
				tmp[i]=gplus[i]-gradnew[i];
			theta=(dot(d,tmp))/sigma;
			
		}
		delta = theta + beta*kappa;
		if (delta <= 0) 
		{
			delta = beta*kappa;
			beta = beta - theta/kappa;
		}
		alpha = - mu/delta;
		for (int i=0; i<xnew.size(); i++)
			xnew[i]=x[i]+alpha*d[i];
      fnew=evalF(xnew);
		Delta=2*(fnew - fold)/(alpha*mu);
		if (Delta  >= 0)
		{
			success = true;
			nsuccess = nsuccess + 1;
			x = xnew;
			fnow = fnew;
		}
		else
		{
			success = false;
			fnow = fold;
		}
		
		if (success==true)
		{
			if ((alpha*norm(d)<gradNorm)&&(fabs(fnew-fold)<fDecrease))
			{
				x=xnew;
            std::cout<<"converged in "<<j<<" iterations with F="<<fnew<<std::endl;
				return 0;
			}
			else
			{
				fold=fnew;
				gradold=gradnew;
            gradnew=evalGrad(x);
			}
			if (norm(gradnew)<eps)
         {
            std::cerr<<"new grad small:"<<"converged in "<<j
               <<" iterations with F="<<fnew<<std::endl;
            return 0;
         }
		}
		if (Delta<0.25)
		{
			beta = min(4.0*beta, betamax);
		}
		
		if (Delta>0.75)
		{
			beta = max(0.5*beta, betamin);
		}
		
		if (nsuccess == x.size())
		{
			for (int i=0;i<d.size();i++)
				d[i] = -gradnew[i];
			nsuccess = 0;
		}
		else
		{
			if (success == 1)
			{
            std::vector<T> tmp;
				tmp.resize(gradnew.size());
				for (int i=0;i<gradnew.size();i++)
					tmp[i]=gradold[i]-gradnew[i];
				
				gamma = dot(tmp,gradnew)/(mu);
				for (int i=0;i<d.size();i++)
					d[i] = gamma*d[i] - gradnew[i];	
			}
		}
		j = j + 1;
	}
   std::cerr << "max iterations reached"<<std::endl;
	return -1;//did not converge
}



#endif
