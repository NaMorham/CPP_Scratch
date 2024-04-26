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
            class StatBase : public Identifiable<StatBase>
            {
            public:
                typedef std::int16_t StatVal;

            private:
                StatVal m_strength;
                StatVal m_dexterity;
                StatVal m_constitution;
                StatVal m_intelligence;
                StatVal m_wisdom;
                StatVal m_charisma;

                StatVal m_strengthAdd;

            public:
                StatBase()
                    : Identifiable("StatBase"),
                      m_strength(0), m_dexterity(0), m_constitution(0),
                      m_intelligence(0), m_wisdom(0), m_charisma(0),
                      m_strengthAdd(0) {}
                StatBase(const StatVal& strength, const StatVal& dexterity, const StatVal& constitution,
                         const StatVal& intelligence, const StatVal& wisdom, const StatVal& charisma,
                         const StatVal& strengthAdd = 0)
                    : Identifiable("StatBase"),
                      m_strength(strength), m_dexterity(dexterity), m_constitution(constitution),
                      m_intelligence(intelligence), m_wisdom(wisdom), m_charisma(charisma),
                      m_strengthAdd(strengthAdd) {}

                StatBase(const StatBase& orig)
                    : Identifiable("StatBase"),
                    m_strength(orig.Strength()), m_dexterity(orig.Dexterity()), m_constitution(orig.Constitution()),
                    m_intelligence(orig.Intelligence()), m_wisdom(orig.Wisdom()), m_charisma(orig.Charisma()),
                    m_strengthAdd(orig.StrengthAdd()) {}

                StatBase(StatBase&&) = delete;

                virtual ~StatBase() {}

                StatBase& operator=(const StatBase& rhs)
                {
                    Strength(rhs.Strength());
                    Dexterity(rhs.Dexterity());
                    Constitution(rhs.Constitution());
                    Intelligence(rhs.Intelligence());
                    Wisdom(rhs.Wisdom());
                    Charisma(rhs.Charisma());
                    StrengthAdd(rhs.StrengthAdd());
                    return *this;
                }

                StatBase& operator=(StatBase&&) = delete;

                [[nodiscard]] inline const StatVal& Strength() const { return m_strength; }
                [[nodiscard]] inline StatVal& Strength() { return m_strength; }
                inline StatBase& Strength(const StatVal& strength) { m_strength = strength; return *this; }

                [[nodiscard]] inline const StatVal& Dexterity() const { return m_dexterity; }
                [[nodiscard]] inline StatVal& Dexterity() { return m_dexterity; }
                inline StatBase& Dexterity(const StatVal& dexterity) { m_dexterity = dexterity; return *this; }

                [[nodiscard]] inline const StatVal& Constitution() const { return m_constitution; }
                [[nodiscard]] inline StatVal& Constitution() { return m_constitution; }
                inline StatBase& Constitution(const StatVal& constitution) { m_constitution = constitution; return *this; }

                [[nodiscard]] inline const StatVal& Intelligence() const { return m_intelligence; }
                [[nodiscard]] inline StatVal& Intelligence() { return m_intelligence; }
                inline StatBase& Intelligence(const StatVal& intelligence) { m_intelligence = intelligence; return *this; }

                [[nodiscard]] inline const StatVal& Wisdom() const { return m_wisdom; }
                [[nodiscard]] inline StatVal& Wisdom() { return m_wisdom; }
                inline StatBase& Wisdom(const StatVal& wisdom) { m_wisdom = wisdom; return *this; }

                [[nodiscard]] inline const StatVal& Charisma() const { return m_charisma; }
                [[nodiscard]] inline StatVal& Charisma() { return m_charisma; }
                inline StatBase& Charisma(const StatVal& charisma) { m_charisma = charisma; return *this; }

                [[nodiscard]] inline const StatVal& StrengthAdd() const { return m_strengthAdd; }
                [[nodiscard]] inline StatVal& StrengthAdd() { return m_strengthAdd; }
                inline StatBase& StrengthAdd(const StatVal& add) { m_strengthAdd = add; return *this; }

                [[nodiscard]] const std::string ToString() const
                {
                    std::stringstream ss;
                    ss << "[ "
                        << "Id: " << Id() << ", "
                        << "Strength: " << Strength() << " + " << StrengthAdd() << ", "
                        << "Dexterity: " << Dexterity() << ", "
                        << "Constitution: " << Constitution() << ", "
                        << "Intelligence: " << Intelligence() << ", "
                        << "Wisdom: " << Wisdom() << ", "
                        << "Charisma: " << Charisma()
                        << " ]";
                    return ss.str();
                }

                [[nodiscard]] const std::string ToJSoNString(bool formatted = true, const size_t depth = 1) const
                {
                    constexpr const char dqu{'\"'};
                    const std::string space{formatted ? " " : ""};
                    const std::string lineEnd{formatted ? "\n" : ""};
                    const std::string indent{formatted ? std::string(depth, '\t') : ""};

                    std::stringstream ss;
                    ss << "{" << space << lineEnd
                       << indent << dqu << "Id" << dqu << ":" << space << Id() << "," << lineEnd
                       << indent << dqu << "Strength" << dqu << ":" << space << Strength() << "," << lineEnd
                       << indent << dqu << "StrengthAdd" << dqu << ":" << space << StrengthAdd() << "," << lineEnd
                       << indent << dqu << "Dexterity: " << dqu << ":" << space << Dexterity() << ", " << lineEnd
                       << indent << dqu << "Constitution: " << dqu << ":" << space << Constitution() << ", " << lineEnd
                       << indent << dqu << "Intelligence: " << dqu << ":" << space << Intelligence() << ", " << lineEnd
                       << indent << dqu << "Wisdom: " << dqu << ":" << space << Wisdom() << ", " << lineEnd
                       << indent << dqu << "Charisma: " << dqu << ":" << space << Charisma() << ", " << lineEnd
                       << (depth > 1 ? std::string(depth - 1, '\t') : "") << "}";
                    return ss.str();
                }

                [[nodiscard]] const bool operator==(const StatBase& rhs) const
                {
                    return ((Strength() == rhs.Strength()) &&
                            (StrengthAdd() == rhs.StrengthAdd()) &&
                            (Dexterity() == rhs.Dexterity()) &&
                            (Constitution() == rhs.Constitution()) &&
                            (Intelligence() == rhs.Intelligence()) &&
                            (Wisdom() == rhs.Wisdom()) &&
                            (Charisma() == rhs.Charisma()));
                }

                [[nodiscard]] const bool operator!=(const StatBase& rhs) const
                {
                    return ((Strength() != rhs.Strength()) ||
                        (StrengthAdd() != rhs.StrengthAdd()) ||
                        (Dexterity() != rhs.Dexterity()) ||
                        (Constitution() != rhs.Constitution()) ||
                        (Intelligence() != rhs.Intelligence()) ||
                        (Wisdom() != rhs.Wisdom()) ||
                        (Charisma() != rhs.Charisma()));
                }

                [[nodiscard]] const StatBase operator+(const StatBase& rhs) const
                {
                    return StatBase{
                        StatVal(Strength() + rhs.Strength()),
                        StatVal(Dexterity() + rhs.Dexterity()),
                        StatVal(Constitution() + rhs.Constitution()),
                        StatVal(Intelligence() + rhs.Intelligence()),
                        StatVal(Wisdom() + rhs.Wisdom()),
                        StatVal(Charisma() + rhs.Charisma()),
                        StatVal(StrengthAdd() + rhs.StrengthAdd())
                    };
                }

                StatBase& operator+=(const StatBase& rhs)
                {
                    Strength(Strength() + rhs.Strength());
                    StrengthAdd(StrengthAdd() + rhs.StrengthAdd());
                    Dexterity(Dexterity() + rhs.Dexterity());
                    Constitution(Constitution() + rhs.Constitution());
                    Intelligence(Intelligence() + rhs.Intelligence());
                    Wisdom(Wisdom() + rhs.Wisdom());
                    Charisma(Charisma() + rhs.Charisma());
                    return *this;
                }

                [[nodiscard]] const StatBase operator-(const StatBase& rhs) const
                {
                    return StatBase{
                        StatVal(Strength() - rhs.Strength()),
                        StatVal(Dexterity() - rhs.Dexterity()),
                        StatVal(Constitution() - rhs.Constitution()),
                        StatVal(Intelligence() - rhs.Intelligence()),
                        StatVal(Wisdom() - rhs.Wisdom()),
                        StatVal(Charisma() - rhs.Charisma()),
                        StatVal(StrengthAdd() - rhs.StrengthAdd())
                    };
                }

                [[nodiscard]] const StatBase operator-() const
                {
                    return StatBase{
                        StatVal(-Strength()),
                        StatVal(-Dexterity()),
                        StatVal(-Constitution()),
                        StatVal(-Intelligence()),
                        StatVal(-Wisdom()),
                        StatVal(Charisma()),
                        StatVal(-StrengthAdd())
                    };
                }

                StatBase& operator-=(const StatBase& rhs)
                {
                    Strength(Strength() - rhs.Strength());
                    StrengthAdd(StrengthAdd() - rhs.StrengthAdd());
                    Dexterity(Dexterity() - rhs.Dexterity());
                    Constitution(Constitution() - rhs.Constitution());
                    Intelligence(Intelligence() - rhs.Intelligence());
                    Wisdom(Wisdom() - rhs.Wisdom());
                    Charisma(Charisma() - rhs.Charisma());
                    return *this;
                }
            };

        }  // end namespace TestObjects

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
