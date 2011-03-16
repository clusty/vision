#include "banana.h"
#include<fstream>
#include<iostream>
#include<string>
#define VERBOSE
using namespace std;

void readData(vector<vector<double> >&F,vector<vector<double> >&x,vector<vector<double> >&y,
      char* mfname,char* xfname,char* xpfname)
{
	x.resize(500);
	y.resize(500);
   for (int i=0;i<x.size();i++)
   {
      x[i].resize(3);
      y[i].resize(3);
   }

   F.resize(3);F[0].resize(3);F[1].resize(3);F[2].resize(3);

	ifstream inf(xfname);
	if (!inf.is_open())
	{
		cout<<"cannot read a file:"<<xfname<<endl;
		return;
	}
	for (int i=0;i<500;i++)
	{
		inf>>x[i][0];
		inf>>x[i][1];
      x[i][2]=1;
	}
	inf.close();
	inf.open(xpfname);
	if (!inf.is_open())
	{
		cout<<"cannot read a file:"<<xpfname<<endl;
		return;
	}
	for (int i=0;i<500;i++)
	{
		inf>>y[i][0];
		inf>>y[i][1];
      y[i][2]=1;

	}	
	inf.close();

   inf.open(mfname);
	if (!inf.is_open())
	{
		cout<<"cannot read a file:"<<mfname<<endl;
		return;
	}

   for (int i=0;i<3;i++)
      for (int j=0;j<3;j++)
         inf>>F[i][j];
   inf.close();

}


main()
{
   rosenbrock r;
   vector<double> x0;
   int ndim = 100;
   x0.resize(ndim);
   for (int i=0;i<ndim;i++)
      x0[i] = 100;

   r.scaledConjugateGradient(x0);
   cout << r.evalF(x0)<<endl;
   
      for (int i=0;i<ndim;i++)
      cout<< x0[i] <<" "; 

}
