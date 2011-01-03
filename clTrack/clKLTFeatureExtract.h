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
	clKLTFeatureExtract(cl_int windowSize = 10);
	~clKLTFeatureExtract();
	void bindData(unsigned imageW, unsigned imageH, cl_float *image, cl_int windowSize);
	void run();
	void getResults(cl_float *h_Output);
private:
	static std::string getSource();
	void initialize();
		
	std::vector<cl::Kernel> _kernels;
	cl::Image2D d_Input, d_OutputDx, d_OutputDy, d_Cornerness;
	cl::Buffer d_KernelDerivative;
	cl_int _patchSize;
	unsigned _imageW, _imageH;
	static bool _initialized;
	//compute the max group size
	static unsigned _blockDim[2];
	static cl_float kernelDerivative[3];
	static const unsigned kSize;
};