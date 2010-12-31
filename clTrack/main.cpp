#include <iostream>
#include "cl2DConvolutionKernelSeparable.h"
int main (int argc, char * const argv[]) {
	std::vector<std::string> kernelNames;
	kernelNames.push_back("ConvolveX");
	kernelNames.push_back("ConvolveY");
	cl2DConvolutionKernelSeparable dummy(kernelNames);
	return dummy.run();
	
}
