/*
 *  ExtractFeatures.h
 *  clTrack
 *
 *  Created by Clusty on 11-01-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include <string>
#include <vector>
#include "cl.hpp"
class clKLTFeatureExtract {
public:	
	clKLTFeatureExtract();
	~clKLTFeatureExtract();
	void bindData(unsigned imageW, unsigned imageH, cl_float *image);
	void run();
	void getResults(cl_float *h_Output);
private:
	static std::string getSource();
	void initialize();
		
	std::vector<cl::Kernel> _kernels;
	cl::Image2D d_Input, d_OutputDx, d_OutputDy;
	cl::Buffer d_KernelDerivative;
	unsigned _imageW, _imageH;
	static bool _initialized;
	//compute the max group size
	static unsigned _blockDim[2];
	static cl_float kernelDerivative[3];
	static const unsigned kSize;
};