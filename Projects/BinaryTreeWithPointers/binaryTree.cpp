#include <iostream>

#include "binaryTreeVer.h"

#include "NaM/testUtils.h"

//-----------------------------------------------------------------------------
using NaM::CppScratch::FirstToken;
using NaM::CppScratch::LastToken;

int main(int argc, char *argv[])
{
    std::cout << FirstToken(LastToken(argv[0], NaM::CppScratch::pathSeparators), ".")
        << " Version " << BinaryTreeWithPointers_VersionFull << std::endl << std::endl;

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------

