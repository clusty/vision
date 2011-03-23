/*
 *  CLDevice.h
 *  clWrapper
 *
 *  Created by Clusty on 11-03-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include<cl.hpp>
class CLDevice{
public:
	CLDevice(const cl::Device);
private:
	cl::Device _clDevice;
};