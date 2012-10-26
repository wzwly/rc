#ifndef __SELFAFX_H__
#define __SELFAFX_H__

#include <stdio.h>
#include <assert.h>
#include <cstdlib>


#if (MSVC)
  //window

#else
  //linux 
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <limits.h>
  #include<sys/stat.h>
  #include<unistd.h>
  #include<sys/mman.h>

  typedef int HANDLE; 
  #define INVALID_HANDLE_VALUE -1

#endif


#if (CMAKE_BUILD_TYPE == Debug)

#define TRACE(a) printf(a)

#else

#define TRACE(a)

#endif

namespace SefAxf
{
  int GetFileSize(const char* pszPath_);
  int GetFileSize(int fd_);
};



#endif 