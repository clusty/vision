#include<cmath>
#include<iostream>
#include <fstream>
#include <string>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>
using namespace std;
using namespace tbb;

class tbb_hisogram{
   public:
      static const int nbins = 255;
      int histogram[ nbins ];
      int *_data;
      tbb_hisogram(int *d):_data(d) 
      {
         for (int i=0;i<nbins;i++)
            histogram[i] = 0;
      }
      void operator()(const blocked_range<int> r)
      {
         int end = r.end();
         for (int i=r.begin();i!=end;++i)
            ++histogram [ _data[i] ];
      }

      tbb_hisogram(const tbb_hisogram &x, split):_data(x._data)
      {
         for (int i=0;i<nbins;i++)
         histogram[i] = 0;
      }

      void join(const tbb_hisogram &y)
      {
         for (int i=0;i<nbins;i++)
         histogram[i] += y.histogram[i];

      }

};

int main()
{
   task_scheduler_init init;
   int *data;
   ifstream in("img.pgm");
   int w,h;
   string junk;
   getline(in, junk);getline(in, junk);
   in>> w;
   in>> h;
   in >> junk;
   data = new int[w*h];
   for (int i=0;i<w*h;i++)
   {
      in >> data[i];
   }

   tbb_hisogram hist(data);

   parallel_reduce(blocked_range<int>(0,w*h), hist,auto_partitioner());


   for (int i=0;i<hist.nbins;i++)
   {
      cout<<hist.histogram[i]<<" ";
   }

   delete[] data;
}
