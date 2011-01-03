#include <iostream>
#include "clKLTFeatureExtract.h"
#include <fstream>
#include <iostream>
using namespace std;
int main (int argc, char * const argv[]) {
	clKLTFeatureExtract kltF;
	int imageW, imageH;
	cout << "reading input"<<endl;
	std::ifstream imag("/Users/clusty/Documents/code/git/vision/clTrack/box.pgm");
	std::string dummy;
	std::getline(imag, dummy);std::getline(imag, dummy);
	imag>>dummy; imageW = atoi(dummy.c_str());
	imag>>dummy; imageH = atoi(dummy.c_str());
	std::getline(imag, dummy);
	cout<<imageW<<" "<<imageH<<endl;
	cl_float *h_Input = new cl_float[4 * imageW * imageH];
	cl_float *h_Output = new cl_float[4 * imageW * imageH];
	for (int i=0;i<imageH * imageW; ++i)
	{
		imag>>h_Input[4*i];
	}
	imag.close();
	
	cout<<"running"<<endl;
	kltF.bindData(imageW, imageH, h_Input);
	kltF.run();
	kltF.getResults(h_Output);
	cout<<"dumping results"<<endl;

	std::ofstream convo("/Users/clusty/Documents/code/git/vision/clTrack/dx.pgm");
	
	convo<<"P2\n#blahblah\n"<<imageW<<" "<<imageH<<std::endl<<"255\n";
	for (int i=0;i<imageH * imageW; ++i) {
		convo<<static_cast<int>(h_Output[4*i])<<" ";
	}
	
	convo.close();
	
	
	delete[] h_Input;
	delete[] h_Output;
}
