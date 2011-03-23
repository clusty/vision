/*
 *  Utils.h
 *  clTrack
 *
 *  Created by Clusty on 11-01-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef CL_UTILS_H
#define CL_UTILS_H
#include "cl.hpp"
#include <string>
static std::string
GetErrorString(cl_int iError)
{
	std::string acErrorString;
    
	switch(iError)
	{
		case(CL_SUCCESS):
			break;
		case(CL_DEVICE_NOT_FOUND):
			acErrorString = "Device not found!";
			break;
		case(CL_DEVICE_NOT_AVAILABLE):
			acErrorString = "Device not available!";
			break;
		case(CL_COMPILER_NOT_AVAILABLE):
			acErrorString = "Device compiler not available!";
			break;
		case(CL_MEM_OBJECT_ALLOCATION_FAILURE):
			acErrorString = "Memory object allocation failure!";
			break;
		case(CL_OUT_OF_RESOURCES):
			acErrorString = "Out of resources!";
			break;
		case(CL_OUT_OF_HOST_MEMORY):
			acErrorString = "Out of host memory!";
			break;
		case(CL_PROFILING_INFO_NOT_AVAILABLE):
			acErrorString = "Profiling information not available!";
			break;
		case(CL_MEM_COPY_OVERLAP):
			acErrorString = "Overlap detected in memory copy operation!";
			break;
		case(CL_IMAGE_FORMAT_MISMATCH):
			acErrorString = "Image format mismatch detected!";
			break;
		case(CL_IMAGE_FORMAT_NOT_SUPPORTED):
			acErrorString = "Image format not supported!";
			break;
		case(CL_INVALID_VALUE):
			acErrorString = "Invalid value!";
			break;
		case(CL_INVALID_DEVICE_TYPE):
			acErrorString = "Invalid device type!";
			break;
		case(CL_INVALID_DEVICE):
			acErrorString = "Invalid device!";
			break;
		case(CL_INVALID_CONTEXT):
			acErrorString = "Invalid context!";
			break;
		case(CL_INVALID_QUEUE_PROPERTIES):
			acErrorString = "Invalid queue properties!";
			break;
		case(CL_INVALID_COMMAND_QUEUE):
			acErrorString = "Invalid command queue!";
			break;
		case(CL_INVALID_HOST_PTR):
			acErrorString = "Invalid host pointer address!";
			break;
		case(CL_INVALID_MEM_OBJECT):
			acErrorString = "Invalid memory object!";
			break;
		case(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR):
			acErrorString = "Invalid image format descriptor!";
			break;
		case(CL_INVALID_IMAGE_SIZE):
			acErrorString = "Invalid image size!";
			break;
		case(CL_INVALID_SAMPLER):
			acErrorString = "Invalid sampler!";
			break;
		case(CL_INVALID_BINARY):
			acErrorString = "Invalid binary!";
			break;
		case(CL_INVALID_BUILD_OPTIONS):
			acErrorString = "Invalid build options!";
			break;
		case(CL_INVALID_PROGRAM):
			acErrorString = "Invalid program object!";
			break;
		case(CL_INVALID_PROGRAM_EXECUTABLE):
			acErrorString = "Invalid program executable!";
			break;
		case(CL_INVALID_KERNEL_NAME):
			acErrorString = "Invalid kernel name!";
			break;
		case(CL_INVALID_KERNEL):
			acErrorString = "Invalid kernel object!";
			break;
		case(CL_INVALID_ARG_INDEX):
			acErrorString = "Invalid index for kernel argument!";
			break;
		case(CL_INVALID_ARG_VALUE):
			acErrorString = "Invalid value for kernel argument!";
			break;
		case(CL_INVALID_ARG_SIZE):
			acErrorString = "Invalid size for kernel argument!";
			break;
		case(CL_INVALID_KERNEL_ARGS):
			acErrorString = "Invalid kernel arguments!";
			break;
		case(CL_INVALID_WORK_DIMENSION):
			acErrorString = "Invalid work dimension!";
			break;
		case(CL_INVALID_WORK_GROUP_SIZE):
			acErrorString = "Invalid work group size!";
			break;
		case(CL_INVALID_GLOBAL_OFFSET):
			acErrorString = "Invalid global offset!";
			break;
		case(CL_INVALID_EVENT_WAIT_LIST):
			acErrorString = "Invalid event wait list!";
			break;
		case(CL_INVALID_EVENT):
			acErrorString = "Invalid event!";
			break;
		case(CL_INVALID_OPERATION):
			acErrorString = "Invalid operation!";
			break;
		case(CL_INVALID_GL_OBJECT):
			acErrorString = "Invalid OpenGL object!";
			break;
		case(CL_INVALID_BUFFER_SIZE):
			acErrorString = "Invalid buffer size!";
			break;
		default:
			acErrorString = "Unknown error!";
			break;
    };
    
	return acErrorString;
}

enum ChannelOrderEnum
{
	R         = 0x10B0,
	A         = 0x10B1,
	Rg        = 0x10B2,
	Ra        = 0x10B3,
	Rgb       = 0x10B4,
	Rgba      = 0x10B5,
	Bgra      = 0x10B6,
	Argb      = 0x10B7,
	Intensity = 0x10B8,
	Luminance = 0x10B9,
};

enum ChannelTypeEnum
{
	SnormInt8      = 0x10D0,
	SnormInt16     = 0x10D1,
	UnormInt8      = 0x10D2,
	UnormInt16     = 0x10D3,
	UnormShort565  = 0x10D4,
	UnormShort555  = 0x10D5,
	UnormInt101010 = 0x10D6,
	SignedInt8     = 0x10D7,
	SignedInt16    = 0x10D8,
	SignedInt32    = 0x10D9,
	UnsignedInt8   = 0x10DA,
	UnsignedInt16  = 0x10DB,
	UnsignedInt32  = 0x10DC,
	HalfFloat      = 0x10DD,
	Float          = 0x10DE,
};

// Wrapping for enumeration: cl_channel_order (ChannelOrderEnum)
static const char* ToString( ChannelOrderEnum enumerationValue )
{
	switch( enumerationValue )
	{
		case R:
			return "R";
		case A:
			return "A";
		case Rg:
			return "Rg";
		case Ra:
			return "Ra";
		case Rgb:
			return "Rgb";
		case Rgba:
			return "Rgba";
		case Bgra:
			return "Bgra";
		case Argb:
			return "Argb";
		case Intensity:
			return "Intensity";
		case Luminance:
			return "Luminance";
	}
	return "Unknown ChannelOrderEnum";
}

// Wrapping for enumeration: cl_channel_type (ChannelTypeEnum)
static const char* ToString( ChannelTypeEnum enumerationValue )
{
	switch( enumerationValue )
	{
		case SnormInt8:
			return "SnormInt8";
		case SnormInt16:
			return "SnormInt16";
		case UnormInt8:
			return "UnormInt8";
		case UnormInt16:
			return "UnormInt16";
		case UnormShort565:
			return "UnormShort565";
		case UnormShort555:
			return "UnormShort555";
		case UnormInt101010:
			return "UnormInt101010";
		case SignedInt8:
			return "SignedInt8";
		case SignedInt16:
			return "SignedInt16";
		case SignedInt32:
			return "SignedInt32";
		case UnsignedInt8:
			return "UnsignedInt8";
		case UnsignedInt16:
			return "UnsignedInt16";
		case UnsignedInt32:
			return "UnsignedInt32";
		case HalfFloat:
			return "HalfFloat";
		case Float:
			return "Float";
	}
	return "Unknown ChannelTypeEnum";
}

#endif