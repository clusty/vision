/*
 *  CLKernel.cpp
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CLKernel.h"
#include<vector>
#include<assert.h>
#include<CLContext.h>
CLKernel::CLKernel(cl::Kernel &ker, const CLContext *ctx):
_clKernel(ker)
{
	_clContext = ctx;
	
	//ker.getInfo( CL_KERNEL_CONTEXT, &_clContext );
	setActiveDevice( 0 );
}

void CLKernel::setActiveDevice(int devId)
{
/*	std::vector<cl::Device> devs;
	_clContext.getInfo(CL_CONTEXT_DEVICES, &devs);
	assert(devId < devs.size());
	_clDevice = devs[devId];*/
}

void CLKernel::run()
{
	
}