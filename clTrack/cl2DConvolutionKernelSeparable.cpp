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
	inFile.close();
	return code;
}

int cl2DConvolutionKernelSeparable::run()
{
	const unsigned int imageW = 1920;
    const unsigned int imageH = 1072;
	//host buffers
	cl_float *h_Input, *h_Output;
	const unsigned int kSize = 3;
	cl_float h_kernel[] = {    0.4038,    0.8021,    0.4038,
		0.8021,   -4.8233,    0.8021,
		0.4038,    0.8021,    0.4038};
	
	h_Input = new cl_float[imageW * imageH];
	h_Output = new cl_float[imageW * imageH];

	std::ifstream imag("/Users/clusty/Documents/code/git/vision/clTrack/B_ANTOINETTE_3DTRACK_LONG.0001.pgm");
	std::string dummy;
	std::getline(imag, dummy);std::getline(imag, dummy);std::getline(imag, dummy);std::getline(imag, dummy);
	std::cout<<dummy;
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
								   kSize*kSize * sizeof(cl_float), h_kernel, &err);
		clInput = cl::Buffer(env->getContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
							 imageH * imageW * sizeof(cl_float), h_Input, &err);
		clOutput = cl::Buffer(env->getContext(), CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, 
							 imageH * imageW * sizeof(cl_float), h_Output, &err);
		
		
		_clKernel.setArg(0, clInput);
		_clKernel.setArg(1, clConvoKernel);
		_clKernel.setArg(2, clOutput);
		_clKernel.setArg(3, imageW);
		_clKernel.setArg(4, kSize);
		

		queue.enqueueNDRangeKernel(_clKernel, cl::NullRange, 
								   cl::NDRange(imageW, imageH), 
								   cl::NDRange(32,16), NULL, &event);
		event.wait();
		queue.enqueueReadBuffer(clOutput,CL_FALSE, 0, imageH * imageW * sizeof(cl_float), h_Output, NULL, &event);
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
	
	std::ofstream orig("/Users/clusty/Documents/code/git/vision/clTrack/orig.pgm");
	std::ofstream convo("/Users/clusty/Documents/code/git/vision/clTrack/convo.pgm");
	
	orig<<"P2\n#blahblah\n"<<imageW<<" "<<imageH<<std::endl<<"255\n";
	convo<<"P2\n#blahblah\n"<<imageW<<" "<<imageH<<std::endl<<"255\n";
	for (int i=0;i<imageH * imageW; ++i) {
		orig<<static_cast<int>(h_Input[i])<<" ";
		convo<<static_cast<int>(h_Output[i])<<" ";
	}
	
	orig.close();
	convo.close();
	queue.enqueueReadBuffer(clConvoKernel,CL_FALSE, 0, kSize * sizeof(cl_float), h_Output, NULL, &event);
	event.wait();

	for (int i=0;i<kSize*kSize;i++)
		std::cout<<h_Output[i]<<" ";
	
	delete[] h_Input;
	delete[] h_Output;
	
	return 0;
}