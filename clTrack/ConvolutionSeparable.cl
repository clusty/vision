__kernel void ConvolveX(const __global  float * pInput,
                        __constant float * pFilter,
                        __global  float * pOutput,
                        const int nFilterWidth)
{
	
    const int nWidth = get_global_size(0);
    const int xOut = get_global_id(0);
    const int yOut = get_global_id(1);
	const int idxOut = yOut * nWidth + xOut;
	if (yOut - trunc(nFilterWidth/2.0f)>0)
	{
		float sum = 0;
		for (int c = 0; c < nFilterWidth; c++)
		{
			sum += pFilter[c]*pInput[ idxOut + c ];
		}
		
    pOutput[idxOut] = sum;
	
	}
	else
	{
		pOutput[idxOut] = 0.0f;
	}
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
	__local float cache[32][16];
	for (int i = 0;i<32;i++)
	for (int j=0;j<16;j++)
	cache[i][j] = pInput[idxOut];
	
    pOutput[idxOut] = cache[0][0];
}