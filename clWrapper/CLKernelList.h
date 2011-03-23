/*
 *  CLKernel.h
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CL_KERNEL_LIST
#define _CL_KERNEL_LIST
#include<vector>
#include<map>
#include<string>
#include<cl.hpp>
#include<CLKernel.h>
class CLContext;

class CLKernelList{
public:
	CLKernelList(std::vector<cl::Kernel> &, const CLContext* = NULL);
	CLKernel& operator[](const std::string &);
private:
	std::map<std::string, CLKernel> _clKernels;
};
#endif
