#include <iostream>
#include <cmath>

#include "NaM/testUtils.h"
#include "NaM/identifiable.h"

#include "NaM/TestObjects/testObjects.h"

class Foo : public NaM::CppScratch::Identifiable<Foo>
{
private:
    std::string m_name{""};
    std::int32_t m_x{0}, m_y{0}, m_z{0};

public:
    Foo() = delete;
    Foo(const std::string& name)
        : NaM::CppScratch::Identifiable<Foo>("Foo"), m_name{name} {}
    Foo(const std::string& name, const std::int32_t& x, const std::int32_t& y, const std::int32_t& z)
        : NaM::CppScratch::Identifiable<Foo>("Foo"), m_name{name}, m_x{x}, m_y{y}, m_z{z} {}
    Foo(const Foo& orig)
        : NaM::CppScratch::Identifiable<Foo>("Foo"), m_name(orig.Name()) {}
    virtual ~Foo() { m_name.clear(); m_x = m_y = m_z = 0; }

    [[nodiscard]] inline const std::string& Name() const { return m_name; }
    [[nodiscard]] inline std::string& Name() { return m_name; }
    inline Foo& Name(const std::string& name) { m_name.assign(name); return *this; }

    [[nodiscard]] inline const std::int32_t& X() const { return m_x; }
    [[nodiscard]] inline std::int32_t& X() { return m_x; }
    inline Foo& X(const std::int32_t& x) { m_x = x; return *this; }

    [[nodiscard]] inline const std::int32_t& Y() const { return m_y; }
    [[nodiscard]] inline std::int32_t& Y() { return m_y; }
    inline Foo& Y(const std::int32_t& y) { m_y = y; return *this; }

    [[nodiscard]] inline const std::int32_t& Z() const { return m_z; }
    [[nodiscard]] inline std::int32_t& Z() { return m_z; }
    inline Foo& Z(const std::int32_t& z) { m_z = z; return *this; }

    [[nodiscard]] inline const double Length() const
    {
        double l{0.0}, x2, y2, z2;
        x2 = std::pow(double(m_x), 2.0);
        y2 = std::pow(double(m_y), 2.0);
        z2 = std::pow(double(m_z), 2.0);
        l = std::sqrt(x2 + y2 + z2);
        return l;
    }

    [[nodiscard]] std::string ToString() const
    {
        std::stringstream ss;
        ss << "[ "
            << "Id: " << Id() << ", "
            << "Name: " << Name() << ", "
            << "Pos (" << X() << ", " << Y() << ", " << Z() << ") "
            << "]";
        return ss.str();
    }    
};

std::ostream& operator<<(std::ostream& oss, const Foo& foo)
{
    oss << foo.ToString();
    return oss;
}

void TestFoo();
void TestDummyData();
void TestPoint2D();

using NaM::CppScratch::TestRunCerr;

using NaM::CppScratch::FirstToken;
using NaM::CppScratch::LastToken;

NaM::CppScratch::_CounterVal g_counter;

// -----------------------------------------------------------------------
int main(int argc, char* argv[])
{
    std::cout << std::endl
        << FirstToken(LastToken(argv[0], NaM::CppScratch::pathSeparators), ".")
        << std::endl << std::endl;

    TestRunCerr run("Main");

    TestFoo();

    std::cout << std::endl;

    TestDummyData();

    std::cout << std::endl;

    TestPoint2D();

    std::cout << std::endl;

    return 0;
}

// -----------------------------------------------------------------------
void TestFoo()
{
    TestRunCerr run("Foo");

    Foo a("A");

    std::cout << g_counter << a << ", Length: " << a.Length() << std::endl;

    Foo b(a);
    b.Name("B");
    b.X(1).Y(0).Z(3);

    std::cout << g_counter << b << ", Length: " << b.Length() << std::endl;

    Foo c("C", 3, 4, 0);

    std::cout << g_counter << c << ", Length: " << c.Length() << std::endl;
}

void TestDummyData()
{
    TestRunCerr run("DummyData");

    NaM::CppScratch::TestObjects::DummyData d;

    std::cout << g_counter << d.ToString() << std::endl;

    NaM::CppScratch::TestObjects::DummyData d2("dd", 42, true);

    std::cout << g_counter << d2.ToString() << std::endl;
}

void TestPoint2D()
{
    TestRunCerr run("Point2D");

    NaM::CppScratch::TestObjects::Point2D p1;

    std::cout << g_counter << p1 << std::endl;
}

