/*
 *  CLKernel.cpp
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CLKernelList.h"

CLKernelList::CLKernelList(std::vector<cl::Kernel> &kers, const CLContext *ctx)
{
	std::string name;
	for(int i=0;i<kers.size();i++)
	{
		kers[i].getInfo(CL_KERNEL_FUNCTION_NAME, &name);
		_clKernels[name] = CLKernel(kers[i], ctx);
	}
}

CLKernel& CLKernelList::operator[](const std::string &kernelName)
{
	return _clKernels[kernelName];
}