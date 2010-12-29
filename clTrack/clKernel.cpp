/*
 *  clKernel.cpp
 *  clTrack
 *
 *  Created by Clusty on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "clKernel.h"
#include <string>
#include "cl.hpp"
#include "clEnvironment.h"
#include <iostream>
clKernel::clKernel():
	_name(), _initialized(false)
{
}

void clKernel::initialize(std::string code)
{
	//dummy kernel
	try {
		cl_int err;
		clEnvironment *env = clEnvironment::getInstance();
		cl::Program::Sources source(1,
									std::make_pair(code.c_str(),code.length()));
		cl::Program program = cl::Program(env->getContext(), source);
		program.build(env->getDevices());
		
		_clKernel = cl::Kernel(program, _name.c_str(), &err);
		
	}
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
	}
	_initialized = true;
}

int clKernel::run()
{
	if (!_initialized)
	{
		initialize(getSource());
	}
	clEnvironment *env = clEnvironment::getInstance();
	cl::CommandQueue queue = env->getQueue();
	
	cl::Event event;
	try{
	queue.enqueueNDRangeKernel( _clKernel, 
								cl::NullRange, 
								cl::NDRange(4,4),
								cl::NullRange,
								NULL,
								&event); 
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
	return 0;
}