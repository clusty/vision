/*
 *  ExtractFeatures.cpp
 *  clTrack
 *
 *  Created by Clusty on 11-01-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "clKLTFeatureExtract.h"
#include "clEnvironment.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;
bool clKLTFeatureExtract::_initialized = false;
unsigned clKLTFeatureExtract::_blockDim[2] = {0,0};
unsigned const clKLTFeatureExtract::kSize = 3;
cl_float clKLTFeatureExtract::kernelDerivative[3] = {1,-2,1};


clKLTFeatureExtract::clKLTFeatureExtract(cl_int windowSize)
:_patchSize(windowSize)
{	
	if (!_initialized)
	{
		initialize();
	}

}

clKLTFeatureExtract::~clKLTFeatureExtract()
{

}

std::string clKLTFeatureExtract::getSource()
{
	ifstream inFile("/Users/clusty/Documents/code/git/vision/clTrack/kltFeatureExtract.cl");
	string code((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();
	return code;
}

void clKLTFeatureExtract::initialize()
{
	try{
		cl_int err;
		clEnvironment *env = clEnvironment::getInstance();
		vector<int> maxGroupSize;
		
		env->getDevices()[0].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES,&maxGroupSize );
		//all possible max group sizes are divisible by 16
		_blockDim[0] = maxGroupSize[0]/16.0;
		_blockDim[1] = 16;
		cout << "max Group Size:"<<_blockDim[0]<<" "<<_blockDim[1]<< endl;;
	
		std::string code = getSource();
		cl::Program::Sources source(1,
								std::make_pair(code.c_str(),code.length()));
		cl::Program program = cl::Program(env->getContext(), source);
		
		try{
			char buildOptions[255];
			sprintf(buildOptions,"-D KERNEL_RADIUS=%u -D WINDOW_SIZE=%u",static_cast<int>(floor(kSize/2.0)),
					_patchSize);
			err = program.build(env->getDevices(), buildOptions);
		}		
		catch (cl::Error err) {
			cout<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(env->getDevices()[0])<<endl;
		}
		
		_kernels.push_back(cl::Kernel(program, "derivativeX", NULL));
		_kernels.push_back(cl::Kernel(program, "derivativeY", NULL));
		_kernels.push_back(cl::Kernel(program, "computeCornerness", NULL));
		_initialized = true;
	}
	
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()<<"("<<GetErrorString(err.err())<<")"
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
	}
}

void clKLTFeatureExtract::bindData(unsigned imageW, unsigned imageH,cl_float *h_Input, cl_int windowSize)
{
	_imageW = imageW;
	_imageH = imageH;
	_patchSize = windowSize;
	clEnvironment *env = clEnvironment::getInstance();
	try{
		d_KernelDerivative = cl::Buffer(env->getContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
								   kSize * sizeof(cl_float), kernelDerivative, NULL);
	
		d_Input = cl::Image2D(env->getContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
							  cl::ImageFormat(CL_RGBA, CL_FLOAT),_imageW, _imageH, 0, h_Input);
		
		d_OutputDx = cl::Image2D(env->getContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, 
							  cl::ImageFormat(CL_RGBA, CL_FLOAT),_imageW, _imageH, 0, NULL);
		
		d_OutputDy = cl::Image2D(env->getContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, 
							  cl::ImageFormat(CL_RGBA, CL_FLOAT),_imageW, _imageH, 0, NULL);
		
	/*	d_Cornerness = cl::Image2D(env->getContext(), CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, 
							  cl::ImageFormat(CL_RGBA, CL_FLOAT),_imageW, _imageH, 0, NULL);*/
	}
	
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()<<"("<<GetErrorString(err.err())<<")"
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
	}
}
void clKLTFeatureExtract::run()
{
	try{
		_kernels[0].setArg(0, d_Input);
		_kernels[0].setArg(1, d_KernelDerivative);
		_kernels[0].setArg(2, d_OutputDx);

		_kernels[1].setArg(0, d_Input);
		_kernels[1].setArg(1, d_KernelDerivative);
		_kernels[1].setArg(2, d_OutputDy);
		
		_kernels[2].setArg(0, d_OutputDx);
		_kernels[2].setArg(1, d_OutputDy);
		_kernels[2].setArg(2, d_Input);
	
		cl::CommandQueue queue = clEnvironment::getInstance()->getQueue();
	
		//enqueue the 2 derivative kernels
		queue.enqueueNDRangeKernel(_kernels[0], cl::NullRange, 
							   cl::NDRange(_imageW, _imageH), 
							   cl::NDRange(_blockDim[0],_blockDim[1]));
	
		queue.enqueueNDRangeKernel(_kernels[1], cl::NullRange, 
							   cl::NDRange(_imageW, _imageH), 
							   cl::NDRange(_blockDim[0],_blockDim[1]));
		
		queue.enqueueNDRangeKernel(_kernels[2], cl::NullRange, 
								cl::NDRange(_imageW, _imageH), 
								cl::NDRange(_blockDim[0],_blockDim[1]));
		
		queue.enqueueBarrier();
		
	}
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()<<"("<<GetErrorString(err.err())<<")"
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
	}
}

void clKLTFeatureExtract::getResults(cl_float *h_Output)
{
	cl::size_t<3> origin, chunk;
	origin[0] = 0; origin[1] = 0; origin[2] = 0; 
	chunk[0] = _imageW; chunk[1] = _imageH; chunk[2] =1;
	cl::CommandQueue queue = clEnvironment::getInstance()->getQueue();
	queue.enqueueReadImage(d_Input,CL_TRUE, origin, chunk,0, 0, h_Output);
}
