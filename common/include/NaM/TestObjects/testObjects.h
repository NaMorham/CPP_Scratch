#pragma once

#include "NaM/identifiable.h"
#include "NaM/testUtils.h"

//------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        namespace TestObjects
        {
            struct DummyData : public NaM::CppScratch::Identifiable<DummyData>
            {
                std::string name;
                std::int32_t val;
                bool is;

                DummyData()
                    : Identifiable("DummyData"),
                    name(""), val(INT32_MAX), is{ false } {}

                DummyData(const std::string& _name, const std::int32_t _val, const bool& _is)
                    : Identifiable("DummyData"),
                    name(_name), val(_val), is{ _is } {}

                DummyData(const DummyData& orig)
                    : Identifiable("DummyData"),
                    name(orig.name), val(orig.val), is(orig.is) {}

                DummyData(DummyData& other)
                    : Identifiable("DummyData"),
                    name(other.name), val(other.val), is(other.is) {}

                virtual ~DummyData()
                {
                    name.clear();
                }

                DummyData& operator=(const DummyData& rhs)
                {
                    name.assign(rhs.name);
                    val = rhs.val;
                    is = rhs.is;
                    return *this;
                }

                DummyData& operator=(DummyData&& rhs) noexcept
                {
                    name = rhs.name;
                    val = rhs.val;
                    is = rhs.is;
                    return *this;
                }

                const std::string ToString() const
                {
                    std::stringstream ss;
                    ss << "[ Id: " << GetID() << ", Name: \"" << name << "\", Value: " << val << ", Is: "
                        << NaM::CppScratch::TrueOrFalse(is) << " ]";
                    return ss.str();
                }

                const std::string ToJSoNString() const
                {
                    std::stringstream ss;
                    ss << "{\"Id\":" << GetID() << ","
                        <<"\"Name\":\"" << name << "\","
                        << "\"Value\":" << val << ","
                        << "\"Is\":"
                        << NaM::CppScratch::TrueOrFalse(is)
                        << "}";
                    return ss.str();
                }
            };

            std::ostream& operator<<(std::ostream& oss, const DummyData& dummy)
            {
                oss << dummy.ToString();
                return oss;
            }
        }  // end namespace TestObjects
    }  // end namespace CppScratch
}  // end namespace NaM

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
