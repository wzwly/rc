#include "sefafx.h"

long int SefAxf::GetFileSize(const char* pszPath_)
{
    struct stat _sBuf;
    if (stat(pszPath_, &_sBuf))
    {
        return -1;
    }

    return (long int)_sBuf.st_size;
}

long int SefAxf::GetFileSize(int fd_)
{
    struct stat _sBuf;
    if (fstat(fd_, &_sBuf))
    {
        return -1;
    }

    return (long int)_sBuf.st_size;
}
