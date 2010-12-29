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

#include<string>
#include "cl.hpp"
class clKernel{
public:
	clKernel();
	virtual	std::string getSource() = 0;
	virtual int run();
	virtual void initialize(std::string code);
	
protected:
	std::string _code;
	cl::Kernel _clKernel;
	std::string _name;
	bool _initialized;
	
};

#endif