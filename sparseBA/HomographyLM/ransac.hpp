#include<utils.h>
#include <boost/numeric/bindings/lapack/driver/gesdd.hpp>//svn
template<class model>
class ransac{
   public:
      ransac(mat &dat)
      void run();
      void getBestFit(mat &);
   private:
      void computeRescaling(mrange &d, vec &scaling) ;
      void conditionData();
      void buildLeastSqMatrix(mat &dat, mat &model);
      bool solveLeastSq(mat &data, mat &model);
      int computeInliers(mat &mod, boost::numeric::ublas::vector<bool> &mask, real_t &cur_std);

      vec _scale1, _scale2;//conditioning of data
      mat _dataScaled;
      mat _M;//big (tall) matrix that we have to solve
      mat _h;//the model params
};

template<class model>
ransac<model>::ransac(mat &data)
{
   
}

template<class model>
void ransac<model, data>::computeRescaling(mrange &d, vec &scaling) 
{
   assert(d.size2()==2);
   scaling.resize(3);
   float cx=0,cy=0;//centroid
   for (int i=0;i<d.size1(); ++i)
   {
      cx+=d(i,0);
      cy+=d(i,1);
   }
   //mean (cx, cy)
   cx/=d.size1();
   cy/=d.size1();
   float distx,disty;
   float mdist=0;
   for (int i=0;i<d.size1(); ++i)
   {
      distx=d(i,0)-cx;
      disty=d(i,1)-cy;
      mdist+=sqrt(distx*distx+disty*disty);
   }
   mdist/=d.size1();//mean distance

   scaling(0) = sqrt(2)/mdist;
   scaling(1) = -cx * sqrt(2) / mdist;
   scaling(2) = -cy * sqrt(2) / mdist; 
}

template<class model,  mat &data>
void ransac<model, data>::conditionData()
{
   assert(data.size2()==4);
   _dataScaled.resize(data.size1(), 4);
   mrange dat1(data, ublas::range(0, data.size1()), ublas::range(0,2));
   mrange dat2(data, ublas::range(0, data.size1()), ublas::range(2,4));

   computeRescaling(dat1, _scale1);
   computeRescaling(dat2, _scale2);

   for (int i=0;i<data.size1();++i)
   {
      _dataScaled(i,0) = data(i,0) * _scale1(0) + _scale1(1);
      _dataScaled(i,1) = data(i,1) * _scale1(0) + _scale1(2);
      _dataScaled(i,2) = data(i,2) * _scale2(0) + _scale2(1);
      _dataScaled(i,3) = data(i,3) * _scale2(0) + _scale2(2);
   }
}


template<class model, mat &data>
void ransac<model, data>::buildLeastSqMatrix(mat &dat, mat &mod)
{
   const int height = dat.size1();
   const int nParams =  model::nParams;
   const int nIndepEq = model::nIndepEq;
   _M.resize(height, nParams);

   for (int i=0;i<height;i += nIndepEq)
   {
      mrange hchunk(_M, ublas::range(i,i+nIndepEq), ublas::range(0,nParams));
      mrow dchunk(dat, i);
      model::leastSqBlock(hchunk, dchunk);
   }

}

template<class model, mat &data>
bool ransac<model, data>::solveLeastSq(mat &dat, mat &mod)
{
   buildLeastSqMatrix(dat, mod);
   const int m = _M.size1();
   const int n = _M.size2();
   const int minmn = m < n ? m : n; 
   vec s(minmn);
   mat u(m, minmn); mat vt(n,n);
   boost::numeric::bindings::lapack::gesdd( 'S', _M, s, u, vt );
   if (s(6)<1e-5)
   {
      printf("bad point choice: linear dependent\n");
      return false;//bad choice of points
   }

   mrange sol(vt, ublas::range(8,9),ublas::range(0,9));//might have to switch (vertical chunk)
   _h.resize(3,3);
   for (int i=0;i<3;++i)
      for (int j=0;j<3;++j)
         _h(i,j) = sol(0,i*3+j);

   //enforce the rank 2
   s.resize(3);
   u.resize(3,3);
   vt.resize(3,3);
   boost::numeric::bindings::lapack::gesdd( 'S', _h, s, u, vt );
   mat sm(3,3);
   for (int i=0;i<3;i++)
     for (int j=0;j<3;j++)
       if (i==j)
	 sm(i,i)=s(i);
       else
	 sm(i,j)=0;
   sm(2,2)=0; 
   _h = ublas::prod (u, mat (ublas::prod (sm, vt))); 

   //revert the scaling
   mat t1(3,3), t2(3,3);
   for (int i=0;i<3;i++)
     for (int j=0;j<3;j++)
     {
        t1(i,j)=0;
        t2(i,j)=0;
     }
   
   t1(0,0)=t1(1,1)=_scale1(0);
   t1(0,2)=_scale1(1);t1(1,2)=_scale1(2);
   t1(2,2)=1;

   t2(0,0)=t2(1,1)=_scale2(0);
   t2(0,2)=_scale2(1);t2(1,2)=_scale2(2);
   t2(2,2)=1;

   //T2'*H*T1
   _h = ublas::prod (ublas::trans(t2), mat (ublas::prod (_h, t1))); 
   return true;

}

template<class model, mat &data>
void ransac<model, data>::run()
{
   int N = 1000, s = model::nParams, sample_cnt = 0;
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
      mat dataSel(s,4);
      for (int i=0;i<s;i++)
	   {
	      dataSel(i,0)=data(cur_idx(i),0);
	      dataSel(i,1)=data(cur_idx(i),1);
	      dataSel(i,2)=data(cur_idx(i),2);
	      dataSel(i,3)=data(cur_idx(i),3);
	   }
      bool solved = solveLeastSq(dataSel, curF);
      // print_m(curF, "data");
      double cur_std;
      if (solved)
	   {
	      int cur_ninliers=
	      computeInliers(curF, cur_inlier_mask, cur_std);
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
      
      bool solved = solveLeastSq(dataSel, _h);
      printf("got %d inliers out of %d points\n", best_ninliers,(int) data.size1());
      print_m(_h,"best F");
   }
   else
      {
         printf("everything is outlier\n");
      }
}

