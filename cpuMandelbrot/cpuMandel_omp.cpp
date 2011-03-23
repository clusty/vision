#include<cmath>
#include <fstream>
#include<omp.h>

using namespace std;
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

void compute(float *buffer)
{
#pragma omp parallel for shared(buffer)
   for (int i=0;i<H;i++)
      for (int j=0;j<W;j++)
         worker(buffer,i,j);
}

int main()
{

//   task_scheduler_init init;
   
   float *buffer = new float[W * H];
   compute(buffer);
   ofstream out("mandel.pgm");
   out<<"P2\n#blahblah\n"<<W<<" "<<H<<std::endl<<MAX_ITER<<endl;
   for (int i=0;i<W*H;i++)
      out<<buffer[i]<<" ";

   out.close();

   delete[] buffer;
}
