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
cl2DConvolutionKernelSeparable::cl2DConvolutionKernelSeparable(std::vector<std::string> name)
{
	_names = name;
}

std::string cl2DConvolutionKernelSeparable::getSource()
{
	std::ifstream inFile("/Users/clusty/Documents/code/git/vision/clTrack/ConvolutionSeparable.cl");
	std::string code((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	return code;
}

int cl2DConvolutionKernelSeparable::run()
{
	const unsigned int imageW = 1920;
    const unsigned int imageH = 1072;
	//host buffers
	cl_float *h_Input, *h_Output;
	/*const unsigned int kSize = 5;
	cl_float h_kernel[] = {     0.0239,    0.0460,    0.0499,    0.0460,    0.0239,
		0.0460,    0.0061,   -0.0923,    0.0061,    0.0460,
		0.0499,   -0.0923,   -0.3182,   -0.0923,    0.0499,
		0.0460,    0.0061,   -0.0923,    0.0061,    0.0460,
		0.0239,    0.0460,    0.0499,    0.0460,    0.0239};*/
	const unsigned int kSize = 3;
	cl_float h_kernel[]={1,-2,1};
	
	h_Input = new cl_float[imageW * imageH];
	h_Output = new cl_float[imageW * imageH];

	std::ifstream imag("/Users/clusty/Documents/code/git/vision/clTrack/box.pgm");
	std::string dummy;
	std::getline(imag, dummy);std::getline(imag, dummy);std::getline(imag, dummy);std::getline(imag, dummy);

	for (int i=0;i<imageH * imageW; ++i)
	{
		imag>>h_Input[i];
	}
	imag.close();
	
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
		clInput = cl::Buffer(env->getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
							 imageH * imageW * sizeof(cl_float), h_Input, &err);
		clOutput = cl::Buffer(env->getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
							 imageH * imageW * sizeof(cl_float), h_Output, &err);
		
		
		_clKernels[0].setArg(0, clInput);
		_clKernels[0].setArg(1, clConvoKernel);
		_clKernels[0].setArg(2, clOutput);
		_clKernels[0].setArg(3, kSize);
		

		queue.enqueueNDRangeKernel(_clKernels[0], cl::NullRange, 
								   cl::NDRange(imageW, imageH), 
								   cl::NDRange(32,16), NULL, &event);
		event.wait();
		
		_clKernels[1].setArg(0, clOutput);
		_clKernels[1].setArg(1, clConvoKernel);
		_clKernels[1].setArg(2, clInput);
		_clKernels[1].setArg(3, kSize);
		
		
		queue.enqueueNDRangeKernel(_clKernels[1], cl::NullRange, 
								   cl::NDRange(imageW, imageH), 
								   cl::NDRange(32,16), NULL, &event);
		event.wait();
		
		queue.enqueueReadBuffer(clInput,CL_FALSE, 0, imageH * imageW * sizeof(cl_float), h_Output, NULL, &event);
		event.wait();
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
	

	std::ofstream convo("/Users/clusty/Documents/code/git/vision/clTrack/convo.pgm");
	
	convo<<"P2\n#blahblah\n"<<imageW<<" "<<imageH<<std::endl<<"255\n";
	for (int i=0;i<imageH * imageW; ++i) {
		convo<<static_cast<int>(h_Output[i])<<" ";
	}
	
	convo.close();

	
	delete[] h_Input;
	delete[] h_Output;
	
	return 0;
}