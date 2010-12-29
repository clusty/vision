#include <iostream>
#include "cl2DConvolutionKernelSeparable.h"
int main (int argc, char * const argv[]) {
	cl2DConvolutionKernelSeparable dummy("Convolve");
	// std::cout << "Hello, World!\n";

	return dummy.run();
	
}
