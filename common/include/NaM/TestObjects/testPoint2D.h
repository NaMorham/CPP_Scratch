#pragma once

#include <cstdint>
#include <sstream>

//------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        namespace TestObjects
        {
            struct Point2D
            {
                std::int32_t x{0}, y{ 0 };

                [[nodiscard]] inline const std::string ToString() const
                {
                    std::stringstream ss;
                    ss << "(" << x << ", " << y << ")";
                    return ss.str();
                }
            };

            std::ostream& operator<<(std::ostream& oss, const Point2D& pt)
            {
                oss << "(" << pt.x << ", " << pt.y << ")";
                return oss;
            }

        }  // end namespace TestObjects

    }  // end namespace CppScratch

}  // end namespace NaM

//------------------------------------------------------------------------
