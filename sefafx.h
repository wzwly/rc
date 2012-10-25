#ifndef __SELFAFX_H__
#define __SELFAFX_H__

#include <stdio.h>
#include <assert.h>
#include <limits.h>
//window

#if (MSVC)


#else

//linux 

typedef int HANDLE; 
#define INVALID_HANDLE_VALUE -1

#endif


#if (CMAKE_BUILD_TYPE == Debug)

#define TRACE(a) printf(a)

#else

#define TRACE(a)

#endif




#endif 