__kernel void ConvolveX(const __global  float * pInput,
                        __constant float * pFilter,
                        __global  float * pOutput,
                        const int nFilterWidth)
{
    const int nWidth = get_global_size(0);

    const int xOut = get_global_id(0);
    const int yOut = get_global_id(1);
	const int idxOut = yOut * nWidth + xOut;
	
	float sum = 0;
	for (int c = 0; c < nFilterWidth; c++)
	{
		sum += pFilter[c]*pInput[ idxOut + c ];
	}

    pOutput[idxOut] = sum;
}

__kernel void ConvolveY(const __global  float * pInput,
                        __constant float * pFilter,
                        __global  float * pOutput,
                        const int nFilterWidth)
{
    const int nWidth = get_global_size(0);
    const int xOut = get_global_id(0);
    const int yOut = get_global_id(1);
	const int idxOut = yOut * nWidth + xOut;
	
    pOutput[idxOut] = pInput[idxOut];
}