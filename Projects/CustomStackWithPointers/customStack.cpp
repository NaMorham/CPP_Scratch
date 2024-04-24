#include <iostream>

#include "customStackVer.h"

std::string LastToken(const std::string& in, const std::string seps = "\\/:")
{
    std::size_t lastpos(in.find_last_of(seps));
    if (in.empty())
    {
        return std::string{""};
    }
    else if (lastpos == std::string::npos)
    {
        return in;
    }
    else
    {
        std::string outString(in.substr(lastpos + 1));
        return outString;
    }
}

int main(int argc, char* argv[])
{
    std::cout << LastToken(argv[0]) << " Version " << CustomStackWithPointers_VersionFull << std::endl;

    return 0;
}

