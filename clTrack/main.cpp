#include <iostream>
#include "cl2DConvolutionKernelSeparable.h"
int main (int argc, char * const argv[]) {
	cl2DConvolutionKernelSeparable dummy("Convolve");
	return dummy.run();
	
}
