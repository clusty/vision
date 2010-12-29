/*
 *  cl2DConvolutionSeparable.h
 *  clTrack
 *
 *  Created by Clusty on 10-12-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CL2D_CONVOLUTION_SEPARABLE
#define _CL2D_CONVOLUTION_SEPARABLE
#include "clKernel.h"
#include "cl.hpp"
class cl2DConvolutionKernelSeparable : public clKernel 
{
public:
	cl2DConvolutionKernelSeparable(std::string name);
	virtual std::string getSource();
	virtual int run();
private:
};
#endif