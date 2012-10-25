#ifndef __SELFAFX_H__
#define __SELFAFX_H__

#include <stdio.h>
#include <assert.h>

//linux 
#if (MSVC)

#define HANDLE int

#else
//window



#endif


#if (CMAKE_BUILD_TYPE == Debug)

#define TRACE(a) printf(a)

#else

#define TRACE(a)

#endif




#endif 