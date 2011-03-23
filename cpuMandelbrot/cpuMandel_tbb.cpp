#include<cmath>
#include <fstream>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>
using namespace std;
using namespace tbb;
#define MAX_ITER 5000
#define BAIL 4
#define W 1024
#define H 768
#define MinRe  -2.0
#define MaxRe  1.0
#define MinIm  -1.2

#define MaxIm  MinIm+(MaxRe-MinRe)*H/W
#define Re_factor  (MaxRe-MinRe)/(W-1)
#define Im_factor  (MaxIm-MinIm)/(H-1)

class mandelCpu{
   public:
      float *buffer;
      mandelCpu(int w, int h)
      {
         buffer = new float[w*h];
      }
      ~mandelCpu()
      {
     //    delete[] buffer;
      }
static inline void worker(float *buffer, int i, int j)
{
   float x = 0;
   float y = 0;
   const float x0 = MaxIm - i * Im_factor;
   const float y0 = MinRe + j * Re_factor;
   int iter;
   for (iter = 0;iter<= MAX_ITER; iter++)
   {
      const float x2 = x*x;
      const float y2 = y*y;
      if (x2+y2 > BAIL)
         break;
      const float xtemp = x2 - y2 + x0;
      y = 2 * x * y + y0;
      x = xtemp;
   }
   iter < MAX_ITER ? 
      buffer[ i * W + j] = iter
         :
      buffer[i * W +j ] = MAX_ITER;

}

void operator()(const blocked_range<int> &range)const
{
   const int end = range.end();
   for (int i=range.begin(); i!=end; ++i )
      for (int j=0;j<W;j++)
         worker(buffer, i,j);
}
}
;


int main()
{

   task_scheduler_init init;
   
  mandelCpu crunch(W, H);
  affinity_partitioner partitioner;
  parallel_for(blocked_range<int>(0,H), crunch, partitioner);
   ofstream out("mandel.pgm");
   out<<"P2\n#blahblah\n"<<W<<" "<<H<<std::endl<<MAX_ITER<<endl;
   for (int i=0;i<W*H;i++)
      out<<crunch.buffer[i]<<" ";

   out.close();

}
