#include <iostream>

#include "testUtilsTestsVer.h"

#include "NaM/testUtils.h"

//-----------------------------------------------------------------------------
using NaM::CppScratch::FirstToken;
using NaM::CppScratch::LastToken;

int main(int argc, char *argv[])
{
    std::cout << FirstToken(LastToken(argv[0], NaM::CppScratch::pathSeparators), ".")
        << " Version " << TestUtilsTests_VersionFull << std::endl << std::endl;

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------

