#pragma once

#include "uniqueTestVer.h"

#include "NaM/identifiable.h"
#include "NaM/testUtils.h"

//-----------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        namespace TestObjects
        {
            class StatBase
            {
                typedef std::int16_t StatVal;
            private:
                StatVal m_strength;
                StatVal m_dexterity;
                StatVal m_constitution;

            public:
                StatBase() : m_strength(0), m_dexterity(0), m_constitution(0) {}
                StatBase(const StatVal& strength, const StatVal& dexterity, const StatVal& constitution)
                    : m_strength(strength), m_dexterity(dexterity), m_constitution(constitution) {}
                StatBase(const StatBase& orig)
                    : m_strength(orig.Strength()), m_dexterity(orig.Dexterity()), m_constitution(orig.Constitution()) {}
                virtual ~StatBase() {}

                [[nodiscard]] inline const StatVal& Strength() const { return m_strength; }
                [[nodiscard]] inline StatVal& Strength() { return m_strength; }
                inline StatBase& Strength(const StatVal& strength) { m_strength = strength; return *this; }

                [[nodiscard]] inline const StatVal& Dexterity() const { return m_dexterity; }
                [[nodiscard]] inline StatVal& Dexterity() { return m_dexterity; }
                inline StatBase& Dexterity(const StatVal& dexterity) { m_dexterity = dexterity; return *this; }

                [[nodiscard]] inline const StatVal& Constitution() const { return m_constitution; }
                [[nodiscard]] inline StatVal& Constitution() { return m_constitution; }
                inline StatBase& Constitution(const StatVal& constitution) { m_constitution = constitution; return *this; }

                [[nodiscard]] const std::string ToString() const
                {
                    std::stringstream ss;
                    ss << "    Strength: " << Strength() << std::endl
                        << "   Dexterity: " << Dexterity() << std::endl
                        << "Constitution: " << Constitution() << std::endl;
                    return ss.str();
                }
            };

        }  // end namespace TestObjects

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
