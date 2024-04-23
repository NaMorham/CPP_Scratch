#include <iostream>
#include <cstdint>
#include <sstream>
#include <set>

#include "uniqueTestVer.h"

//-----------------------------------------------------------------------------
typedef std::uint64_t TypeID;

//-----------------------------------------------------------------------------
inline const TypeID _GetId()
{
    static TypeID lastId = 0;
    return ++lastId;
}

template <typename T>
inline const TypeID GetId() noexcept
{
    static TypeID typeId = _GetId();
    return typeId;
}

//-----------------------------------------------------------------------------
// Another test class using the same IdManager but differnt Ids
class IdManager
{
private:
    TypeID sm_id{0};

public:
    [[nodiscard]] TypeID GetNewId() { return ++sm_id; }
};

//-----------------------------------------------------------------------------
// Another test class using the same IdManager but differnt Ids
class Test1
{
private:
    static IdManager sm_idm;
    static const TypeID sm_typeId;

    std::uint64_t m_id;
    std::string m_name;

public:
    [[nodiscard]] static const TypeID GetTypeID() { return sm_typeId; }

    Test1(std::string name) : m_id(Test1::sm_idm.GetNewId()), m_name(name) { }

    [[nodiscard]] const std::uint64_t Id() const { return m_id; }
    [[nodiscard]] const std::string Name() const { return m_name; }

};

IdManager Test1::sm_idm = IdManager();
const TypeID Test1::sm_typeId = GetId<Test1>();

//-----------------------------------------------------------------------------
// Another test class using the same IdManager but differnt Ids
class Test2
{
private:
    static IdManager sm_idm;
    static const TypeID sm_typeId;

    TypeID m_id;
    std::string m_name;

public:
    [[nodiscard]] static const TypeID GetTypeID() { return sm_typeId; }

    Test2(std::string name) : m_id(Test2::sm_idm.GetNewId()), m_name(name) { }

    [[nodiscard]] const std::uint64_t Id() const { return m_id; }
    [[nodiscard]] const std::string Name() const { return m_name; }

};

IdManager Test2::sm_idm = IdManager();
const TypeID Test2::sm_typeId = GetId<Test2>();

//-----------------------------------------------------------------------------
template<typename T>
class Identifiable
{
private:
    static IdManager sm_idm;
    static const TypeID sm_typeId;
    static std::string sm_typename;
    TypeID m_id;

protected:
    Identifiable(const std::string& name)
        : m_id(Identifiable<T>::sm_idm.GetNewId()) { sm_typename.assign(name); }

public:
    [[nodiscard]] inline const TypeID GetID() const { return m_id; }
    [[nodiscard]] inline const TypeID GetTypeID() const { return sm_typeId; }
    [[nodiscard]] inline const std::string& GetTypeName() const { return sm_typename; }
};

template<typename T>
IdManager Identifiable<T>::sm_idm = IdManager();

template<typename T>
const TypeID Identifiable<T>::sm_typeId = GetId<T>();

template<typename T>
std::string Identifiable<T>::sm_typename{""};

//-----------------------------------------------------------------------------
class Test3 : public Identifiable<Test3>
{
private:
    std::uint32_t m_value;
    std::string m_dummy;

protected:

public:
    Test3(const std::uint32_t value = 0, const std::string& dummy = "")
        : Identifiable<Test3>("Test3"), m_value(value), m_dummy(dummy) {}

    Test3(const Test3& orig) = delete;  // no copying
    Test3& operator=(const Test3& rhs) = delete;

    Test3(Test3&& origMv) = delete;  // no moving
    Test3& operator=(const Test3&& rhs) = delete;

    virtual ~Test3() { m_dummy.clear(); }

    [[nodiscard]] inline operator const std::uint32_t() const { return m_value; }
    [[nodiscard]] inline operator const std::string() const { return m_dummy; }

    // accessors/mutators
    [[nodiscard]] inline const char* const Dummy() const { return m_dummy.c_str(); }
    Test3& Dummy(const std::string& newDummy) { m_dummy.assign(newDummy); return *this; }

    [[nodiscard]] inline const std::uint32_t& Value() const { return m_value; }
    Test3& Dummy(const std::uint32_t& newValue) { m_value = newValue; return *this; }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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

class Test4 : public Identifiable<Test4>, public StatBase
{
private:

protected:

public:
    Test4() : Identifiable<Test4>("Test4"), StatBase(1, 2, 3) {}
    virtual ~Test4() {}

    [[nodiscard]] const std::string ToString() const
    {
        std::stringstream ss;
        ss << "[" << GetTypeName() << ":" << GetTypeID() << "]" << std::endl
           << "\tID: " << GetID() << std::endl
           << "\tStats: [" << StatBase::ToString() << "]" << std::endl;
        return ss.str();
    }

    [[nodiscard]] const std::string ToJSoNString(bool formatted = true) const
    {
        constexpr char dqu{'\"'};
        const std::string space{formatted ? " " : ""};
        const std::string lineEnd{formatted ? "\n" : ""};
        const std::string indent{formatted ? "\t" : ""};

        std::stringstream ss;
        ss << "{" << lineEnd
           << indent << dqu << "Type" << dqu << ":" << space << "{" << lineEnd
           << indent << indent << dqu << "Name" << dqu << ":" << space << dqu << GetTypeName() << dqu << "," << lineEnd
           << indent << indent << dqu << "Id" << dqu << ":" << space << GetTypeID() << lineEnd
           << indent << "}," << lineEnd
           << indent << dqu << "Id" << dqu << ":" << space << GetID() << "," << lineEnd
           << indent << dqu << "Stats" << dqu << ":" << space << "[" << lineEnd
           << indent << indent << "{" << space << dqu << "Strength" << dqu << ":" << space << Strength() << space << "}," << lineEnd
           << indent << indent << "{" << space << dqu << "Dexterity" << dqu << ":" << space << Dexterity() << space << "}," << lineEnd
           << indent << indent << "{" << space << dqu << "Constitution" << dqu << ":" << space << Constitution() << space << "}" << lineEnd
           << indent << "]" << lineEnd
           << "}";
        return ss.str();
    }
};

//-----------------------------------------------------------------------------
// Provide some dummy output stream operators
std::ostream& operator<<(std::ostream& oss, const Test1& test) {
    oss << "Type Id:" << test.GetTypeID() << ", Id: " << test.Id()
        << ", Name: " << test.Name();
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const Test2& test) {
    oss << "Type Id:" << test.GetTypeID() << ", Id: " << test.Id()
        << ", Name: " << test.Name();
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const Test3& test) {
    oss << "Type (" << test.GetTypeName() << ":" << test.GetTypeID() << "), Id: "
        << test.GetID() << ", Our value: " << test.Value();
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const Test4& test) {
    oss << test.ToString();
    return oss;
}

//-----------------------------------------------------------------------------
// Simple main test calls
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
        std::string outString(in.substr(lastpos+1));
        return outString;
    }
}

bool AddToSet(const TypeID& id, std::set<TypeID>& theSet)
{
    if (theSet.contains(id))
    {
        return false;
    }
    else
    {
        theSet.insert(id);
        return true;
    }
}

int main(int argc, char *argv[])
{
    const std::string dashes(70, '-');

    std::cout << LastToken(argv[0]) << " Version " << UniqueIdTest_Version_Major << "."
        << UniqueIdTest_Version_Minor << "." << UniqueIdTest_Version_Patch
        << std::endl << std::endl;

    {
        std::cout << dashes << std::endl << "class Test1:" << std::endl;

        std::set<TypeID> Test1IdSet;
        bool test1SameTypeId{ false };
        bool test1DiffInternalId{ false };

        // Each Test1 object should have the same type Id, but a unique internal Id
        Test1 test1_1("test1_1"), test1_2("test1_2"), test1_3("test1_3"), test1_4("test1_4");
        std::cout
            << "\t" << test1_1 << std::endl
            << "\t" << test1_2 << std::endl
            << "\t" << test1_3 << std::endl
            << "\t" << test1_4 << std::endl
            << std::endl;
        test1SameTypeId = ((test1_1.GetTypeID() == test1_2.GetTypeID()) &&
            (test1_3.GetTypeID() == test1_4.GetTypeID()) &&
            (test1_1.GetTypeID() == test1_3.GetTypeID()));
        std::cout << "\tAll Test1 objects have the same type Id? "
            << TrueOrFalse(test1SameTypeId) << std::endl;
        test1DiffInternalId = true;
        test1DiffInternalId &= AddToSet(test1_1.Id(), Test1IdSet);
        test1DiffInternalId &= AddToSet(test1_2.Id(), Test1IdSet);
        test1DiffInternalId &= AddToSet(test1_2.Id(), Test1IdSet);
        test1DiffInternalId &= AddToSet(test1_4.Id(), Test1IdSet);

        std::cout << "\tAll Test1 objects have the different internal Ids? "
            << TrueOrFalse(test1DiffInternalId) << std::endl;

        std::cout << std::endl << std::endl;
        std::cout << dashes << std::endl << "class Test2:" << std::endl;

        std::set<TypeID> Test2IdSet;
        bool test2SameTypeId{ false };
        bool test2DiffInternalId{ false };

        // Each Test1 object should have the same type Id, but a unique internal Id
        // The type Id must be different to Test 1, but the internal Ids are allowed
        // to overlap.  The combination of TypeId AND Id is required to be unique
        Test2 test2_1("test2_1"), test2_2("test2_2"), test2_3("test2_3"), test2_4("test2_4");
        std::cout
            << "\t" << test2_1 << std::endl
            << "\t" << test2_2 << std::endl
            << "\t" << test2_3 << std::endl
            << "\t" << test2_4 << std::endl
            << std::endl;

        test2SameTypeId = ((test2_1.GetTypeID() == test2_2.GetTypeID()) &&
            (test2_3.GetTypeID() == test2_4.GetTypeID()) &&
            (test2_1.GetTypeID() == test2_3.GetTypeID()));

        std::cout << "\tAll Test2 objects have the same type Id? "
            << TrueOrFalse(test2SameTypeId) << std::endl;

        test2DiffInternalId = true;
        test2DiffInternalId &= AddToSet(test2_1.Id(), Test2IdSet);
        test2DiffInternalId &= AddToSet(test2_2.Id(), Test2IdSet);
        test2DiffInternalId &= AddToSet(test2_3.Id(), Test2IdSet);
        test2DiffInternalId &= AddToSet(test2_4.Id(), Test2IdSet);

        std::cout << "\tAll Test2 objects have the different internal Ids? "
            << TrueOrFalse(test2DiffInternalId) << std::endl;
        std::cout << std::endl << std::endl;
    }

    {
        std::cout << dashes << std::endl << "class Test3:" << std::endl;

        std::set<TypeID> Test3IdSet;
        bool test3SameTypeId{ false };
        bool test3DiffInternalId{ false };

        // Each Test1 object should have the same type Id, but a unique internal Id
        Test3 test3_1(1000, "test3_1"), test3_2(2000, "test3_2"),
              test3_3(3000, "test3_3"), test3_4(4000, "test3_4");
        std::cout
            << "\t" << test3_1 << std::endl
            << "\t" << test3_2 << std::endl
            << "\t" << test3_3 << std::endl
            << "\t" << test3_4 << std::endl
            << std::endl;

        test3SameTypeId = ((test3_1.GetTypeID() == test3_2.GetTypeID()) &&
            (test3_3.GetTypeID() == test3_4.GetTypeID()) &&
            (test3_1.GetTypeID() == test3_3.GetTypeID()));
        std::cout << "\tAll Test3 objects have the same type Id? "
            << TrueOrFalse(test3SameTypeId) << std::endl;

        test3DiffInternalId = true;
        test3DiffInternalId &= AddToSet(test3_1.GetID(), Test3IdSet);
        test3DiffInternalId &= AddToSet(test3_2.GetID(), Test3IdSet);
        test3DiffInternalId &= AddToSet(test3_3.GetID(), Test3IdSet);
        test3DiffInternalId &= AddToSet(test3_4.GetID(), Test3IdSet);

        std::cout << "\tAll Test3 objects have the different internal Ids? "
            << TrueOrFalse(test3DiffInternalId) << std::endl;

        std::cout << std::endl << std::endl;
        std::cout << dashes << std::endl << "class Test3:" << std::endl;

        std::set<TypeID> Test4IdSet;
        bool test4SameTypeId{ false };
        bool test4DiffInternalId{ false };

        // Each Test4 object should have the same type Id, but a unique internal Id
        // The type Id must be different to Test 3, but the internal Ids are allowed
        // to overlap.  The combination of TypeId AND Id is required to be unique
        Test4 test4_1, test4_2, test4_3, test4_4;

        std::cout
            << test4_1 << std::endl
            << test4_2 << std::endl
            << test4_3 << std::endl
            << test4_4 << std::endl
            << std::endl;

        test4SameTypeId = ((test4_1.GetTypeID() == test4_2.GetTypeID()) &&
            (test4_3.GetTypeID() == test4_4.GetTypeID()) &&
            (test4_1.GetTypeID() == test4_3.GetTypeID()));

        std::cout << "\tAll Test2 objects have the same type Id? "
            << TrueOrFalse(test4SameTypeId) << std::endl;

        test4DiffInternalId = true;
        test4DiffInternalId &= AddToSet(test4_1.GetID(), Test4IdSet);
        test4DiffInternalId &= AddToSet(test4_2.GetID(), Test4IdSet);
        test4DiffInternalId &= AddToSet(test4_3.GetID(), Test4IdSet);
        test4DiffInternalId &= AddToSet(test4_4.GetID(), Test4IdSet);

        std::cout << "\tAll Test4 objects have the different internal Ids? "
            << TrueOrFalse(test4DiffInternalId) << std::endl;
        std::cout << std::endl << std::endl;

        {
            std::cout << test4_1.ToJSoNString(true) << std::endl;

            std::cout << "{\"TestVals\":["
                << test4_1.ToJSoNString(false) << ","
                << test4_2.ToJSoNString(false) << ","
                << test4_3.ToJSoNString(false) << ","
                << test4_4.ToJSoNString(false)
                << "]}"
                << std::endl;
        }
    }


    std::cout << dashes << std::endl;

    return 0;
}

//-----------------------------------------------------------------------------
