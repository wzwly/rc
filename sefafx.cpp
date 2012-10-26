#include "sefafx.h"

int SefAxf::GetFileSize(const char* pszPath_)
{
  struct stat _sBuf;
  if (stat(pszPath_, &_sBuf))
  {
    return -1;
  }
  
  return (int)_sBuf.st_size;
}

int SefAxf::GetFileSize(int fd_)
{
  struct stat _sBuf;
  if (fstat(fd_, &_sBuf))
  {
    return -1;
  }
  
  return (int)_sBuf.st_size;
}