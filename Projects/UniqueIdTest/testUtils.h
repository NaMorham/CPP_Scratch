#pragma once

#include <iostream>
#include <sstream>

namespace NaM
{
    namespace CppScratch
    {
        const char* TrueOrFalse(const bool& value)
        {
            return value ? "true" : "false";
        }

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

        std::string FirstToken(const std::string& in, const std::string seps = "\\/:")
        {
            std::size_t firstpos(in.find_first_of(seps));
            if (in.empty())
            {
                return std::string{""};
            }
            else if (firstpos == std::string::npos)
            {
                return in;
            }
            else
            {
                std::string outString(in.substr(firstpos + 1));
                return outString;
            }
        }

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
