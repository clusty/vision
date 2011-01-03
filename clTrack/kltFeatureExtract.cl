const sampler_t sampler1 = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP_TO_EDGE;
const sampler_t sampler2 = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP;
__kernel void derivativeX (__read_only image2d_t input, __constant float *pFilter,
							__write_only image2d_t output)
{
	float sum = 0;
	for (int i=-KERNEL_RADIUS;i<=KERNEL_RADIUS;i++)
	{
		int2 coord = {get_global_id(0)+i, get_global_id(1)};
		float4 color = read_imagef(input, sampler1, coord);
		sum+=color.x*pFilter[i+KERNEL_RADIUS];
	}
	write_imagef(output, (int2)(get_global_id(0), get_global_id(1)), (float4)(sum,0,0,0) );
}
__kernel void derivativeY (__read_only image2d_t input, __constant float *pFilter,
							__write_only image2d_t output)
{
	float sum = 0;
	for (int i=-KERNEL_RADIUS;i<=KERNEL_RADIUS;i++)
	{
		int2 coord = {get_global_id(0), get_global_id(1)+i};
		float4 color = read_imagef(input, sampler1, coord);
		sum+=color.x*pFilter[i+KERNEL_RADIUS];
	}
	write_imagef(output, (int2)(get_global_id(0), get_global_id(1)), (float4)(sum,0,0,0) );
}

//computes eigenvalues of a 2x2 matrix
//x y
//z w
float2 computeEigenvalues(float4 m)
{
	float bigSqrt = sqrt(m.x * m.x - 2 * m.x * m.w + 4 * m.y * m.z + m.w * m.w ) * 0.5;
	float term2 = 0.5 * (m.x + m.w);
	return (float2)(bigSqrt + term2, -bigSqrt+term2);
}

__kernel void computeCornerness(__read_only image2d_t dx, __read_only image2d_t dy, __constant int window,
									__write_only image2d_t cornerness)
{
	float4 jac={0,0,0,0};
	for (int i=-WINDOW_SIZE;i<=WINDOW_SIZE;i++)
	{
		for (int j=-WINDOW_SIZE;j<=WINDOW_SIZE;j++)
		{
			int2 coord = {get_global_id(0)+i, get_global_id(1)+j};
			float4 colorDx = read_imagef(dx, sampler2, coord);
			float4 colorDy = read_imagef(dy, sampler2, coord);
			float xx=colorDx.x * colorDx.x;
			float xy=colorDx.x * colorDy.x;
			float yy=colorDy.x * colorDy.x;
			float4 c = {xx,xy,xy,yy};
			jac+=c;
		}
	}
	//normalize
	jac = jac / (float4)((WINDOW_SIZE + 1) * (WINDOW_SIZE + 1));
	//alternatively use the cheaper det(J) + trace^2 (J)
	float2 eigs = computeEigenvalues(jac);
	write_imagef(cornerness, (int2)(get_global_id(0),get_global_id(1)), __clampf4((float4)(length(eigs),0,0,0),0.0,255.0 ));

}
