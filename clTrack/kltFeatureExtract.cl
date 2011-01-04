const sampler_t sampler1 = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP_TO_EDGE;
const sampler_t sampler2 = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP;
__kernel void derivativeX (__read_only image2d_t input, __constant float *pFilter,
							__write_only image2d_t output)
{
	const int2 coord = {get_global_id(0), get_global_id(1)};
	float sum = 0;
	for (int i=-KERNEL_RADIUS;i<=KERNEL_RADIUS;i++)
	{
		float4 color = read_imagef(input, sampler1, coord+(int2)(i,0));
		sum+=color.x*pFilter[i+KERNEL_RADIUS];
	}
	write_imagef(output, coord, (float4)(sum) );
}
__kernel void derivativeY (__read_only image2d_t input, __constant float *pFilter,
							__write_only image2d_t output)
{
	const int2 coord = {get_global_id(0), get_global_id(1)};
	float sum = 0;
	for (int i=-KERNEL_RADIUS;i<=KERNEL_RADIUS;i++)
	{
		float4 color = read_imagef(input, sampler1, coord+(int2)(0,i));
		sum+=color.x*pFilter[i+KERNEL_RADIUS];
	}
	write_imagef(output, coord, (float4)(sum) );
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
#define CORNERNESS_THRESH 1
__kernel void computeCornerness(__read_only image2d_t dx, __read_only image2d_t dy, __write_only image2d_t cornerness)
{
	float4 colorDx, colorDy;
	float4 jac={0,0,0,0};
	const int2 coord = {get_global_id(0), get_global_id(1)};
	for (int i=-WINDOW_SIZE;i<=WINDOW_SIZE;i++)
	{
		for (int j=-WINDOW_SIZE;j<=WINDOW_SIZE;j++)
		{
			int2 lcoord = coord+(int2)(i,j);
			colorDx = read_imagef(dx, sampler2, lcoord);
			colorDy = read_imagef(dy, sampler2, lcoord);
			float xx=colorDx.x * colorDx.x;
			float xy=colorDx.x * colorDy.x;
			float yy=colorDy.x * colorDy.x;
			jac+= (float4)(xx,xy,xy,yy);
		}
	}
	//normalize
	jac = jac / (float4)((WINDOW_SIZE + 1) * (WINDOW_SIZE + 1));
	//float l = jac.x * jac.w - jac.y * jac.z + jac.x * jac.x + jac.w * jac.w;
	//alternatively use the cheaper det(J) + trace^2 (J)
	float2 eigs = computeEigenvalues(jac);
	
	//threshold output
	float4 color = (eigs.x> CORNERNESS_THRESH) && (eigs.y> CORNERNESS_THRESH) ? length(eigs):0;
	write_imagef(cornerness, coord, color);
}
