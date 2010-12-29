/*
 *  cl2DConvolutionSeparable.cpp
 *  clTrack
 *
 *  Created by Clusty on 10-12-28.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "cl2DConvolutionKernelSeparable.h"
#include "clEnvironment.h"

#include <iostream>
#include <fstream>
cl2DConvolutionKernelSeparable::cl2DConvolutionKernelSeparable(std::string name)
{
	_name = name;
}

std::string cl2DConvolutionKernelSeparable::getSource()
{
	std::ifstream inFile("/Users/clusty/Documents/code/git/vision/clTrack/ConvolutionSeparable.cl");
	std::string code((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	return code;
}

int cl2DConvolutionKernelSeparable::run()
{
	const unsigned int imageW = 1920;
    const unsigned int imageH = 1080;
	//host buffers
	cl_float *h_Input, *h_Output;
	cl_float h_kernel[] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
	const unsigned int kSize = 7;
	h_Input = new cl_float[imageW * imageH];
	h_Output = new cl_float[imageW * imageH];
	
	srand(2009);
#pragma omp parallel for
	for (int i=0;i<imageH * imageW; ++i)
	{
		h_Input[i] = (cl_float)(rand() % 16);
	}
	
	if (!_initialized)
	{
		initialize(getSource());
	}
	clEnvironment *env = clEnvironment::getInstance();
	cl::CommandQueue queue = env->getQueue();
	
	cl::Buffer clInput, clOutput, clConvoKernel;
	
	cl_int err;

	cl::Event event;
	
	try{
		clConvoKernel = cl::Buffer(env->getContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
								   kSize * sizeof(cl_float), h_kernel, &err);
		clInput = cl::Buffer(env->getContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
							 imageH * imageW * sizeof(cl_float), h_Input, &err);
		clOutput = cl::Buffer(env->getContext(), CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, 
							 imageH * imageW * sizeof(cl_float), h_Output, &err);
		
		
		_clKernel.setArg(0, clInput);
		_clKernel.setArg(1, clConvoKernel);
		_clKernel.setArg(2, clOutput);
		_clKernel.setArg(3, imageW);
		_clKernel.setArg(4, kSize);
		
		queue.enqueueNDRangeKernel( _clKernel, 
								   cl::NullRange, 
								   cl::NDRange(4,4),
								   cl::NullRange,
								   NULL,
								   &event); 
		event.wait();
		std::cout<<"ran"<<std::endl;
	}
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
		return -1;
	} 
	
	delete[] h_Input;
	delete[] h_Output;
	
	return 0;
}