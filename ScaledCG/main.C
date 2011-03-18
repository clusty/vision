#include "banana.h"
#include<fstream>
#include<iostream>
#include<string>
#define VERBOSE
using namespace std;

main()
{
   int ndim = 100;
   rosenbrock<double> r(ndim);
   vector<double> x0;

   r.initialize();
   x0.resize(ndim);
   for (int i=0;i<ndim;i++)
   {
      x0[i] = (rand()-50000000)/100000.0;
      cout<<x0[i]<<" ";
   }

   r.scaledConjugateGradient(x0, 1000000);
   cout << endl<<r.evalF(x0)<<endl;
   
      for (int i=0;i<ndim;i++)
      cout<< x0[i] <<" "; 

}
