#include <iostream>
#include <cstdint>

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

//-----------------------------------------------------------------------------
// Simple main test calls
const char* TrueOrFalse(const bool& value)
{
    return value ? "true" : "false";
}

int main(int argc, char *argv[])
{
    {
        bool test1SameTypeId{ false };
        bool test1DiifInternalId{ false };

        // Each Test1 object should have the same type Id, but a unique internal Id
        Test1 test1_1("test1_1"), test1_2("test1_2"), test1_3("test1_3"), test1_4("test1_4");
        std::cout
            << test1_1 << std::endl
            << test1_2 << std::endl
            << test1_3 << std::endl
            << test1_4 << std::endl
            << std::endl;
        test1SameTypeId = ((test1_1.GetTypeID() == test1_2.GetTypeID()) &&
            (test1_3.GetTypeID() == test1_4.GetTypeID()) &&
            (test1_1.GetTypeID() == test1_3.GetTypeID()));
        std::cout << "All Test1 objects have the same type Id? "
            << TrueOrFalse(test1SameTypeId) << std::endl;
        test1DiifInternalId = (test1_1.Id() != test1_2.Id() != test1_2.Id() != test1_4.Id());
        std::cout << "All Test1 objects have the different internal Ids? "
            << TrueOrFalse(test1DiifInternalId) << std::endl;
        std::cout << std::endl << std::endl;
    }

    {
        bool test2SameTypeId{ false };
        bool test2DiifInternalId{ false };

        // Each Test1 object should have the same type Id, but a unique internal Id
        // The type Id must be different to Test 1, but the internal Ids are allowed
        // to overlap.  The combination of TypeId AND Id is required to be unique
        Test2 test2_1("test2_1"), test2_2("test2_2"), test2_3("test2_3"), test2_4("test2_4");
        std::cout
            << test2_1 << std::endl
            << test2_2 << std::endl
            << test2_3 << std::endl
            << test2_4 << std::endl
            << std::endl;
        test2SameTypeId = ((test2_1.GetTypeID() == test2_2.GetTypeID()) &&
            (test2_3.GetTypeID() == test2_4.GetTypeID()) &&
            (test2_1.GetTypeID() == test2_3.GetTypeID()));
        std::cout << "All Test1 objects have the same type Id? "
            << TrueOrFalse(test2SameTypeId) << std::endl;
        test2DiifInternalId = (test2_1.Id() != test2_2.Id() != test2_2.Id() != test2_4.Id());
        std::cout << "All Test1 objects have the different internal Ids? "
            << TrueOrFalse(test2DiifInternalId) << std::endl;
        std::cout << std::endl << std::endl;
    }

    return 0;
}

//-----------------------------------------------------------------------------
