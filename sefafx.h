#ifndef __SELFAFX_H__
#define __SELFAFX_H__

#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>

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
  #include<ctype.h>

  typedef int HANDLE; 
  #define INVALID_HANDLE_VALUE -1

#endif


#if (CMAKE_BUILD_TYPE == Debug)

#define TRACE  printf

#else

#define TRACE(a)

#endif

namespace SefAxf
{
  long int GetFileSize(const char* pszPath_);
  long int GetFileSize(int fd_);
};



#endif 