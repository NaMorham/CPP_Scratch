#include <iostream>
#include <cstdint>

#include <algorithm>

//-----------------------------------------------------------------------------
template<std::size_t N>
struct CompTimeStr {
    char data[N] {};

    consteval CompTimeStr(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }

    consteval bool operator==(const CompTimeStr<N> str) const {
        return std::equal(str.data, str.data+N, data);
    }

    template<std::size_t N2>
    consteval bool operator==(const CompTimeStr<N2> s) const {
        return false;
    }

    template<std::size_t N2>
    consteval CompTimeStr<N+N2-1> operator+(const CompTimeStr<N2> str) const {
        char newchar[N+N2-1] {};
        std::copy_n(data, N-1, newchar);
        std::copy_n(str.data, N2, newchar+N-1);
        return newchar;
    }

    consteval char operator[](std::size_t n) const {
        return data[n];
    }

    consteval std::size_t size() const {
        return N-1;
    }
};

template<std::size_t s1, std::size_t s2>
consteval auto operator+(CompTimeStr<s1> fs, const char (&str) [s2]) {
    return fs + CompTimeStr<s2>(str);
}

template<std::size_t s1, std::size_t s2>
consteval auto operator+(const char (&str) [s2], CompTimeStr<s1> fs) {
    return CompTimeStr<s2>(str) + fs;
}

template<std::size_t s1, std::size_t s2>
consteval auto operator==(CompTimeStr<s1> fs, const char (&str) [s2]) {
    return fs == CompTimeStr<s2>(str);
}

template<std::size_t s1, std::size_t s2>
consteval auto operator==(const char (&str) [s2], CompTimeStr<s1> fs) {
    return CompTimeStr<s2>(str) == fs;
}

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
template<typename T, CompTimeStr name>
class Identifiable
{
private:
	static IdManager sm_idm;
	static const TypeID sm_typeId;
	static std::string sm_typename;
	TypeID m_id;

protected:
	Identifiable() : m_id(Identifiable<T, name>::sm_idm.GetNewId()) {}
	virtual ~Identifiable() {}

public:
	[[nodiscard]] inline const TypeID GetID() const { return m_id; }
	[[nodiscard]] inline const TypeID GetTypeID() const { return sm_typeId; }
    [[nodiscard]] inline const std::string& GetTypeName() const { return sm_typename; }
};

template<typename T, CompTimeStr name>
IdManager Identifiable<T,name>::sm_idm = IdManager();

template<typename T, CompTimeStr name>
const TypeID Identifiable<T,name>::sm_typeId = GetId<T>();

template<typename T, CompTimeStr name>
std::string Identifiable<T,name>::sm_typename(name);

class Test3 : public Identifiable<Test3,"Test3">
{
private:
    std::uint32_t m_value;
    std::string m_dummy;

protected:

public:
	Test3(const std::uint32_t value = 0, const std::string& dummy = "")
        : Identifiable<Test3, "Test3">(), m_value(value), m_dummy(dummy) {}

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
