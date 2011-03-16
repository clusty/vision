#include"optimFunction.h"
#include<math.h>
#include<iostream>
using namespace std;

vector<double> optimFunction::evalGrad(vector<double>&x,double h)//compute finite difference gradient (central difference)
{
   vector<double>grad(x.size());
	vector<double> xh=x;
	for (int i=0;i<x.size();i++)
	{

      xh[i] += h;    
      double fxph = evalF(xh);
      xh[i] -= 2*h; // -h brings us back to original val and another -h at -h
      double fxmh = evalF(xh);
      grad[i] = (fxph - fxmh)/ (2*h);


      xh[i] +=h; //bring back component so we are at +0
	}
   return grad;
}

int optimFunction::scaledConjugateGradient(vector<double>&x,int niter,double gradNorm,
      double fDecrease)//stop when ALL gradient is too small, function decrease is small
            //or did all the iterations

{
	int nsuccess=0,j;
	double h=1e-7;
	vector<double> gradnew,gradold,d,xplus,gplus,xnew;
	double fnew,fold,fnow,beta,betamin,betamax,gamma;
	double mu,kappa,sigma,theta,delta,alpha;
	double Delta;
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
				cerr << "small gradient:"<<"converged in "<<j<<" iterations with F="<<fnew<<endl;
				return 0;//gradient norm smaller than eps
			}
			sigma=sigma0/sqrt(kappa);
			for (int i=0;i<xplus.size();i++)
				xplus[i] = x[i] + sigma*d[i];
         gplus=evalGrad(xplus);
			vector<double> tmp;
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
				cout<<"converged in "<<j<<" iterations with F="<<fnew<<endl;
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
            cerr<<"new grad small:"<<"converged in "<<j<<" iterations with F="<<fnew<<endl;
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
				vector<double> tmp;
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
	cerr << "max iterations reached"<<endl;
	return -1;//did not converge
}


