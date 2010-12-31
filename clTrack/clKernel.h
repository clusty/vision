/*
 *  clKernel.h
 *  clTrack
 *
 *  Created by Clusty on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *  
 *  Dummy kernel example
 *
 */
#ifndef _CL_KERNEL
#define _CL_KERNEL

#include "cl.hpp"
class clKernel{
public:
	clKernel();
	virtual	std::string getSource() = 0;
	virtual int run();
	virtual void initialize(std::string code);
	
protected:
	std::vector<cl::Kernel> _clKernels;
	std::vector<std::string> _names;
	bool _initialized;
	
};

#endif