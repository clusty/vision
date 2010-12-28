/*
 *  initCLEnvironment.h
 *  clTrack
 *
 *  Created by Clusty on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CL_ENVIRONMENT
#define _CL_ENVIRONMENT

#include<vector>
#include "cl.hpp"
class clEnvironment
{
public:
	static clEnvironment* getInstance();
	cl::Context &getContext() {return _clGPUContext;}
	cl::CommandQueue &getQueue() {return _clQueue;}
	std::vector<cl::Device> &getDevices() {return _clGPUDevices; }
private:
	clEnvironment();
	static clEnvironment* _clEnvironment;
	std::vector<cl::Platform> _clPlatforms;
	std::vector<cl::Device> _clGPUDevices;
	cl::Context _clGPUContext;
	cl::CommandQueue _clQueue;
};
#endif