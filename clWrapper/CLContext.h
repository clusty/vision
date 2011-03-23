/*
 *  CLContext.h
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CL_CONTEXT
#define _CL_CONTEXT
#include <cl.hpp>
#include <CLDevice.h>
#include <CLKernelList.h>
#include <vector>
#include <string>

class CLContext{
public:
	enum contextType {
		GPU = CL_DEVICE_TYPE_GPU,
		CPU = CL_DEVICE_TYPE_CPU,
		ALL = CL_DEVICE_TYPE_ALL
	};
	CLContext(int type = GPU);
	CLKernelList buildProgramFromSource(const std::string &code,const std::string &opts = std::string(""));
private:
	cl::Context _clContext;
	std::vector<cl::Platform> _clPlatforms;
	std::vector<cl::Device> _clDevices;
};

#endif