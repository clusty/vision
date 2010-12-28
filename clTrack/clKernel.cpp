/*
 *  clKernel.cpp
 *  clTrack
 *
 *  Created by Clusty on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#define __CL_ENABLE_EXCEPTIONS
#include "clKernel.h"
#include <string>
#include "cl.hpp"
#include "clEnvironment.h"
#include <iostream>
clKernel::clKernel()
{
	//dummy kernel
	_code = std::string ("__kernel void hello(void) "
						"{ "
							"  "
						"} ");
	try {
		cl_int err;
		clEnvironment *env = clEnvironment::getInstance();
		cl::Program::Sources source(1,
									std::make_pair(_code.c_str(),_code.length()));
		cl::Program program = cl::Program(env->getContext(), source);
		program.build(env->getDevices());
	
		_clKernel = cl::Kernel(program, "hello", &err);
		
	}
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()
		<< "("
		<< err.err()
		<< ")"
		<< std::endl;
	}
}

int clKernel::run()
{
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