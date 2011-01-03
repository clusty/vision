const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP_TO_EDGE;
__kernel void derivativeX (__read_only image2d_t input, __constant float *pFilter,
							__write_only image2d_t output)
{
	float sum = 0;
	float4 outColor = {0,0,0,0};
	for (int i=-KERNEL_RADIUS;i<=KERNEL_RADIUS;i++)
	{
		int2 coord = {get_global_id(0)+i, get_global_id(1)};
		float4 color = read_imagef(input, sampler, coord);
		sum+=color.x*pFilter[i+KERNEL_RADIUS];
	}
	outColor.x = sum;
	int2 out= {get_global_id(0), get_global_id(1)};
	write_imagef(output, out, outColor );
}
__kernel void derivativeY (__read_only image2d_t input, __constant float *pFilter,
							__write_only image2d_t output)
{
	float sum = 0;
	float4 outColor = {0,0,0,0};
	for (int i=-KERNEL_RADIUS;i<=KERNEL_RADIUS;i++)
	{
		int2 coord = {get_global_id(0), get_global_id(1)+i};
		float4 color = read_imagef(input, sampler, coord);
		sum+=color.x*pFilter[i+KERNEL_RADIUS];
	}
	outColor.x = sum;
	int2 out= {get_global_id(0), get_global_id(1)};
	write_imagef(output, out, outColor );
}
