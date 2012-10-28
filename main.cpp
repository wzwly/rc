#include <iostream>
#include "./Core/CoreHead.h"


int main ( int argc, char **argv )
{
    CFileIndex* _pFileIndex = NULL;
    _pFileIndex = new CFileIndex();
    _pFileIndex->Create ( "test.nc" );
    delete _pFileIndex;

    std::cout << "Hello,  world!" << std::endl;
    return 0;
}
