#pragma once
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned long uint32;
typedef signed long int32;
typedef unsigned long long uint64;
typedef signed long long int64;

namespace core
{
	constexpr uint32 MAX_ENTITIES{ 5000000 };
}

#define DEBUG_LOG(msg) \
	std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl