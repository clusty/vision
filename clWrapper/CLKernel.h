/*
 *  CLKernel.h
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CL_KERNEL
#define _CL_KERNEL
#include<cl.hpp>

class CLContext;

class CLKernel{
public:
	CLKernel( cl::Kernel&, const CLContext*  );
	CLKernel() {};
	void operator()(void);
	void setActiveDevice(int devId);//pass CLContext for queue management
private:
	cl::Kernel _clKernel;
	cl::Device _clDevice;
	//cl::Context  _clContext;
	const CLContext *_clContext;
	void run();
};
#endif