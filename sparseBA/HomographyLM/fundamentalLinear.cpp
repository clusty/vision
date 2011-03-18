#include <boost/numeric/bindings/ublas/vector.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/lapack/driver/gesdd.hpp>//svn
#include <boost/numeric/ublas/io.hpp>
#include "utils.h"//for prints
#include <string>
#include <fstream>
using namespace boost::numeric::bindings;
using namespace boost::numeric::ublas;
using std::size_t; 
using std::cout;
using std::endl;
using std::string;
using std::ifstream; 
namespace ublas = boost::numeric::ublas;

typedef double real_t; 
typedef ublas::matrix<real_t, ublas::column_major> mat;
typedef matrix_range<mat> mrange;
typedef matrix_row<matrix<real_t,ublas::column_major > > mrow;
typedef ublas::vector<real_t> vec;

void loadData(string fname,  mat &data)
{
  ifstream ifile(fname.c_str());
  if (!ifile)
    cout<<"cannot read"<<endl;
  int num=0;
  while (!ifile.eof())
  {
     data.resize(num+1,4, true);
     ifile>>data(num,0);ifile>>data(num,1);ifile>>data(num,2);ifile>>data(num,3);
     num++;
  }
  ifile.close();
}

//compute point rescaling such that centroid is 0 and mean dist is sqrt(2)
//scaling = (cx,cy, scale)
void computeRescaling(mrange &data, vec &scaling)
{
  assert(data.size2()==2);
  scaling.resize(3);
  float cx=0,cy=0;//centroid
  for (int i=0;i<data.size1(); ++i)
  {
    cx+=data(i,0);
    cy+=data(i,1);
  }
  //mean (cx, cy)
  cx/=data.size1();
  cy/=data.size1();
  float distx,disty;
  float mdist=0;
  for (int i=0;i<data.size1(); ++i)
  {
    distx=data(i,0)-cx;
    disty=data(i,1)-cy;
    mdist+=sqrt(distx*distx+disty*disty);
  }
  mdist/=data.size1();//mean distance

  scaling(0) = sqrt(2)/mdist;
  scaling(1) = -cx * sqrt(2) / mdist;
  scaling(2) = -cy * sqrt(2) / mdist; 
}

void conditionData(mat &data, mat &dataScaled, vec &s1, vec &s2)
{
  assert(data.size2()==4);
  dataScaled.resize(data.size1(), 4);
  mrange dat1(data, range(0, data.size1()), range(0,2));
  mrange dat2(data, range(0, data.size1()), range(2,4));

  computeRescaling(dat1, s1);
  computeRescaling(dat2, s2);

  for (int i=0;i<data.size1();++i)
  {
    dataScaled(i,0) = data(i,0) * s1(0) + s1(1);
    dataScaled(i,1) = data(i,1) * s1(0) + s1(2);
    dataScaled(i,2) = data(i,2) * s2(0) + s2(1);
    dataScaled(i,3) = data(i,3) * s2(0) + s2(2);
  }
}
void fundamentalBlock(mrange &f, mrow &d)
{
  assert(f.size1()==1 && f.size2()==9);
  float x = d(0), y=d(1), xp=d(2), yp=d(3);
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

void buildLeastSqFundamentalMatrix(mat &data, mat &h)
{
  int height = data.size1();
  h.resize(height, 9);
  for (int i=0;i<height;i++)
  {
    mrange hchunk(h, range(i,i+1), range(0,9));
    mrow dchunk(data, i);
    fundamentalBlock(hchunk, dchunk);
  }
}

bool solveLeastSqFundamental(mat &data, mat &h)
{
  mat dataS;//scaled data
  mat H;//big least Sq matrix
  vec s1,s2;
  conditionData(data, dataS, s1, s2);
  buildLeastSqFundamentalMatrix(dataS,H);
  const int m = H.size1();
  const int n = H.size2();
  const int minmn = m < n ? m : n; 
  vec s(minmn);
  mat u(m, minmn); mat vt(n,n);

  //least square solution
  lapack::gesdd( 'S', H, s, u, vt );
  if (s(6)<1e-5)
    {
      printf("bad point choice: linear dependent\n");
      return false;//bad choice of points
    }
  //solution is last singular vector
  mrange sol(vt, range(8,9),range(0,9));//might have to switch (vertical chunk)
  h.resize(3,3);
  for (int i=0;i<3;++i)
    for (int j=0;j<3;++j)
      h(i,j) = sol(0,i*3+j);

  //enforce the rank 2
   s.resize(3);
   u.resize(3,3);
   vt.resize(3,3);
   lapack::gesdd( 'S', h, s, u, vt );
   mat sm(3,3);
   for (int i=0;i<3;i++)
     for (int j=0;j<3;j++)
       if (i==j)
	 sm(i,i)=s(i);
       else
	 sm(i,j)=0;
   sm(2,2)=0; 
  h = ublas::prod (u, mat (ublas::prod (sm, vt))); 

 //revert the scaling
 mat t1(3,3), t2(3,3);
   for (int i=0;i<3;i++)
     for (int j=0;j<3;j++)
     {
        t1(i,j)=0;
        t2(i,j)=0;
     }
   t1(0,0)=t1(1,1)=s1(0);
   t1(0,2)=s1(1);t1(1,2)=s1(2);
   t1(2,2)=1;

   t2(0,0)=t2(1,1)=s2(0);
   t2(0,2)=s2(1);t2(1,2)=s2(2);
   t2(2,2)=1;

   //T2'*H*T1
   h = ublas::prod (ublas::trans(t2), mat (ublas::prod (h, t1))); 
   return true;
}

int computeNoInliersFundamental(mat &data, mat &f, ublas::vector<bool> &inlier_mask, double &dist_std)
{
  double T_DIST = 0.5;//dist>0.1 pixels = outlier might want to make it sqrt(5.99)*sigma
  int num_inlier = 0;
  double m_dist=0;
  dist_std=0;
  vec dist(data.size1());
  inlier_mask.resize(data.size1());
  for (int i=0;i<data.size1();i++)
    inlier_mask(i)=false;
  for (int i=0;i<data.size1();i++)
    {
      vec x1(3),x2(3);
      x1(0)=data(i,0);x1(1)=data(i,1);x1(2)=1;
      x2(0)=data(i,2);x2(1)=data(i,3);x2(2)=1;
      dist(i) = fabs(inner_prod(prod (x2, f),x1));//x2 F x1
      if (dist(i)<T_DIST)
	{
	  num_inlier++;
	  inlier_mask(i)=true;
	  m_dist+=dist(i);
	}
    }
  m_dist = m_dist/ num_inlier;
  for (int i=0;i<data.size1();i++)
      if (inlier_mask(i))
	dist_std += pow(dist(i)-m_dist,2);
  dist_std = dist_std/(num_inlier-1);
  return num_inlier;
}
void ransacFundamental(mat &data, mat &h)
{
  int N = 1000, s = 9, sample_cnt = 0;
  const double p = 0.99;
  double best_std; 
  int best_numinlier, best_ninliers=-1;
  ublas::vector<bool> best_inlier_mask(data.size1());
  ublas::vector<bool> cur_inlier_mask(data.size1());
  ublas::vector<int> cur_idx(s);
  mat curF, bestF;
  best_ninliers = -1;
  srand(134);
  while(N > sample_cnt)
  {
    bool badCombo = true;
    while (badCombo)
    {
      badCombo = false;
      for (int i=0;i<s;i++)
      {
	cur_idx(i) = rand() % data.size1();
	for (int j=0;j<i;j++)
	if (cur_idx(i)==cur_idx(j))
	{
	    badCombo = true;
	    break;
	}
      }
      //print_v(cur_idx, "idx");
      //copy the data
      mat dataSel(s,4);
      for (int i=0;i<s;i++)
	{
	  dataSel(i,0)=data(cur_idx(i),0);
	  dataSel(i,1)=data(cur_idx(i),1);
	  dataSel(i,2)=data(cur_idx(i),2);
	  dataSel(i,3)=data(cur_idx(i),3);
	}
      bool solved = solveLeastSqFundamental(dataSel, curF);
      // print_m(curF, "data");
      double cur_std;
      if (solved)
	{
	  int cur_ninliers=
	    computeNoInliersFundamental(data, curF, cur_inlier_mask, cur_std);
	  //more inliers or same inliers but better dist std
	  if (cur_ninliers>best_ninliers || cur_ninliers==best_ninliers && cur_std<best_std)
	    {
	      bestF=curF;
	      best_std = cur_std;
	      best_inlier_mask = cur_inlier_mask;
	      best_ninliers = cur_ninliers;
	    }
	  double e = 1 - cur_ninliers / (double) data.size1();
	  N = (int)(log(1-p)/log(1-pow(1-e,s)));
	  sample_cnt++;
	}
    }
  }

   if (best_ninliers>8)
   {
     mat dataSel;
     dataSel.resize(best_ninliers, 4);
     int idx = 0;
     for (int i=0;i<data.size1();i++)
       if (best_inlier_mask(i))
       {
	 dataSel(idx,0)=data(i,0);
	 dataSel(idx,1)=data(i,1);
	 dataSel(idx,2)=data(i,2);
	 dataSel(idx,3)=data(i,3);
	 idx++;
	}
      bool solved = solveLeastSqFundamental(dataSel, h);
      printf("got %d inliers out of %d points\n", best_ninliers,(int) data.size1());
      print_m(h,"best F");
    }
   else
     {
       printf("everything is outlier\n");
     }

}
int main()
{
   mat data;
   mat f;
   loadData("data/corr.01.03.txt",data);
  //print_m(data, "data");
   solveLeastSqFundamental(data, f);
   ransacFundamental(data, f);
}
