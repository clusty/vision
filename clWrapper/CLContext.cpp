/*
 *  CLContext.cpp
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <CLContext.h>
#include <Utils.h>
#include <iostream>
#include <assert.h>
CLContext::CLContext(int type)
{
#ifdef __CL_ENABLE_EXCEPTIONS
	try {
#endif
   cl::Platform::get(&_clPlatforms);		
	//gpu context using platform 0. need to manage if multiple platforms
	_clPlatforms[0].getDevices(static_cast<cl_device_type>(type), &_clDevices);
	_clContext = cl::Context(_clDevices);
#ifdef __CL_ENABLE_EXCEPTIONS
	}
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()
		<< "("
		<< err.err()<<"-"<<GetErrorString(err.err())
		<< ")"
		<< std::endl;
	}
#endif
}

CLKernelList CLContext::buildProgramFromSource(const std::string &code,const std::string &opts)
{
	assert(_clDevices.size()>0);
	std::vector< std::pair <const char*, size_t> > sources;
	//pass all the files to compile all of them
	sources.push_back(std::make_pair(code.c_str(), code.size()));
	cl::Program prog(_clContext, sources);
	std::vector<cl::Kernel> kernels;
#ifdef __CL_ENABLE_EXCEPTIONS
	try {
#endif
		prog.build(_clDevices, opts.c_str());
		prog.createKernels(&kernels);
#ifdef __CL_ENABLE_EXCEPTIONS
	}
	catch (cl::Error err) {
		std::cerr  << "ERROR: "
		<< err.what()
		<< "("
		<< err.err()<<"-"<<GetErrorString(err.err())
		<< ")"<<std::endl;
		for(int i=0;i<_clDevices.size();i++)
		{
			std::cout<<	prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_clDevices[0])<<std::endl;
		}
	}
#endif
	return CLKernelList(kernels, this);
}