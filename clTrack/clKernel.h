/*
 *  clKernel.h
 *  clTrack
 *
 *  Created by Clusty on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CL_KERNEL
#define _CL_KERNEL

#include<string>
#include "cl.hpp"
class clKernel{
public:
	clKernel();
	int run();
protected:
	std::string _code;
	cl::Kernel _clKernel;
	
};

#endif