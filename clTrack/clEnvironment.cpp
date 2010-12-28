/*
 *  initCLEnvironment.cpp
 *  clTrack
 *
 *  Created by Clusty on 10-12-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#define __CL_ENABLE_EXCEPTIONS
#include "clEnvironment.h"
#include "cl.hpp"
#include <vector>
#include <iostream>
#include <string>
#define DEBUG

clEnvironment* clEnvironment::_clEnvironment = 0;

clEnvironment::clEnvironment()
{
	try {
		cl_int err = CL_SUCCESS;
		err = cl::Platform::get(&_clPlatforms);

	
		//gpu context using platform 0
		cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(_clPlatforms[0])(), 0};
		_clGPUContext = cl::Context(CL_DEVICE_TYPE_GPU, properties);
		_clGPUDevices = _clGPUContext.getInfo<CL_CONTEXT_DEVICES>();
		
		//make a queue for each device
		_clQueue = cl::CommandQueue(_clGPUContext, _clGPUDevices[0], 0, &err);
#ifdef DEBUG
		std::cout << "Platforms"<<std::endl;
		for (int i=0; i<_clPlatforms.size(); ++i) {
			std::string param;
			_clPlatforms[i].getInfo(CL_PLATFORM_NAME, &param);
			std::cout<<i <<" "<<param<<" ";
			_clPlatforms[i].getInfo(CL_PLATFORM_VERSION, &param);
			std::cout<<" "<<param<<" ";
			_clPlatforms[i].getInfo(CL_PLATFORM_EXTENSIONS, &param);
			std::cout<<" "<<param<<std::endl;
		}
		std::cout << "GPU Devices"<<std::endl;
		for (int i=0; i<_clGPUDevices.size(); ++i) {
			std::string param;
			_clGPUDevices[i].getInfo(CL_DEVICE_NAME, &param);
			std::cout<<i <<" "<<param<<" "<<std::endl;
			
		}
#endif
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
clEnvironment* clEnvironment::getInstance()
{
	if (!_clEnvironment)
	{
		_clEnvironment = new clEnvironment();
	}
	return _clEnvironment;
}