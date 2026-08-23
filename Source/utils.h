#pragma once
#pragma warning(disable: 4005)
#pragma warning(disable: 4098)


#ifndef ArraySize
	#define ArraySize(a) (sizeof(a) / sizeof((a)[0]))
#endif

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;