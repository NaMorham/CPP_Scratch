#include <iostream>
#include <iomanip>
#include <list>

#include "testUtilsTestsVer.h"

#include "NaM/testUtils.h"

//-----------------------------------------------------------------------------
//using NaM::CppScratch::DASHES;
//using NaM::CppScratch::EQUALS;
//using NaM::CppScratch::HASHES;
using NaM::CppScratch::nullvalstr;
using NaM::CppScratch::pathSeparators;
using NaM::CppScratch::wsSeparators;
using NaM::CppScratch::lineEndSeparators;

using NaM::CppScratch::TrueOrFalse;
using NaM::CppScratch::PtrString;
using NaM::CppScratch::TextNumLength;
using NaM::CppScratch::XofYStr;
using NaM::CppScratch::Path::FileShortName;

struct TestGroupResult
{
    size_t numPassed{ 0 }, numTests{ 0 };
    [[nodiscard]] inline bool Passed() const { return (numPassed == numTests); }
    inline TestGroupResult& operator&=(const bool result) { if (result) ++numPassed; return *this; }
};

[[nodiscard]] TestGroupResult TestTrueOrFalse();
[[nodiscard]] TestGroupResult TestXofYStr();
[[nodiscard]] TestGroupResult TestPtrString();
[[nodiscard]] TestGroupResult TestTextNumLength();
[[nodiscard]] TestGroupResult TestFirstToken();
[[nodiscard]] TestGroupResult TestLastToken();
[[nodiscard]] TestGroupResult TestSplitLastToken();
[[nodiscard]] TestGroupResult TestSplitFirstToken();
[[nodiscard]] TestGroupResult TestPath_BaseName();
[[nodiscard]] TestGroupResult TestPath_DirName();
[[nodiscard]] TestGroupResult TestPath_FileShortName();
[[nodiscard]] TestGroupResult TestPath_AppendDelimiter();
[[nodiscard]] TestGroupResult TestPath_StripDelimiter();

//-----------------------------------------------------------------------------
using NaM::CppScratch::TestRunCerr;
NaM::CppScratch::_CounterVal g_counter;

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    std::cout << FileShortName(argv[0])
        << " Version " << TestUtilsTests_VersionFull << std::endl << std::endl;

    (void)TestTrueOrFalse();
    (void)TestXofYStr();
    (void)TestTextNumLength();
    (void)TestPtrString();
    (void)TestFirstToken();
    (void)TestLastToken();
    (void)TestSplitLastToken();
    (void)TestSplitFirstToken();
    (void)TestPath_BaseName();
    (void)TestPath_DirName();
    (void)TestPath_FileShortName();
    (void)TestPath_AppendDelimiter();
    (void)TestPath_StripDelimiter();

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
const char* const PassFail(const bool result)
{
    static constexpr const char* const _passStr{ "PASS" };
    static constexpr const char* const _failStr{ "FAIL" };
    return result ? _passStr : _failStr;
}

std::string TestNumLabel(size_t x, size_t y)
{
    std::stringstream ss;
    ss << "Test [" << XofYStr(x, y) << "]";
    return ss.str();
}

TestGroupResult TestTrueOrFalse()
{
    TestRunCerr run("Test bool to string : TrueOrFalse(...)");
    std::string got;
    bool result;
    TestGroupResult runResult{ 0, 2 };

    got.assign(TrueOrFalse(true));
    result = (got.compare("true") == 0);
    runResult &= result;
    std::cerr << g_counter << "Test [1/2]: TrueOrFalse(true) -> \"" << got
        << "\" == " << PassFail(result) << std::endl;

    got.assign(TrueOrFalse(false));
    result = (got.compare("false") == 0);
    runResult &= result;
    std::cerr << g_counter << "Test [2/2]: TrueOrFalse(false) -> \"" << got
        << "\" == " << PassFail(result) << std::endl;

    return runResult;
}

TestGroupResult TestTextNumLength()
{
    TestRunCerr run("Get text length of number");

    bool result;
    TestGroupResult runResult;
    return runResult;
}

TestGroupResult TestXofYStr()
{
    TestRunCerr run("Get text X of Y");

    struct TestVal
    {
        std::string name;
        size_t x, y;
        std::string expected;
        std::string sep{""};
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;
    std::string got;

    tests.push_back({ "zeros", 0, 0, "0/0", ""});
    tests.push_back({ "zeros with single char sep", 0, 0, "0:0", ":" });
    tests.push_back({ "zeros with string sep", 0, 0, "0<:>0", "<:>" });
    tests.push_back({ "single digits", 2, 7, "2/7", "" });
    tests.push_back({ "single digits with single char sep", 2, 7, "2+7", "+" });
    tests.push_back({ "single digits with string sep", 2, 7, "2-+-7", "-+-" });
    tests.push_back({ "double digits", 15, 24, "15/24", "" });
    tests.push_back({ "double digits with single char sep", 21, 99, "21@99", "@" });
    tests.push_back({ "double digits with string sep", 33, 99, "33\\\\99", "\\\\" });

    tests.push_back({ "diff size numbers", 3, 1234, "   3/1234", "" });
    tests.push_back({ "diff size numbers with sep", 12, 999, " 12:999", ":" });

    tests.push_back({ "revered values", 7, 3, "3/7", "" });

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": ";
        if (test.sep.empty())
            got.assign(XofYStr(test.x, test.y));
        else
            got.assign(XofYStr(test.x, test.y, test.sep));
        result = (test.expected.compare(got) == 0);
        std::cerr << test.name << " = \"" << got << "\" -> " << PassFail(result);
        if (!result)
            std::cerr << ", expected \"" << test.expected << "\"";
        std::cerr << std::endl;
    }

    return runResult;
}

TestGroupResult TestPtrString()
{
    TestRunCerr run("Pointer to string");

    struct TestVal
    {
        std::string name;
        const void* const inPtr;
        std::string expected;
    };

    bool result;
    TestGroupResult runResult;
    std::list<TestVal> tests;
    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };

    for (const TestVal& test : tests)
    {
        // there is no pass fail here since the different OS' seem to provide a different string
        // we could calculate for the architecture, but it seems like a hassle
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size());
        if (!result)
            std::cerr << ", expected \"" << test.expected << "\"";
        std::cerr << std::endl;

        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestFirstToken()
{
    using NaM::CppScratch::FirstToken;
    TestRunCerr run("Return first token");

    struct TestVal
    {
        std::string name;
        std::string inVal;
        std::string expected;
        const std::string *seps;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    tests.push_back({ "Empty string","","",nullptr });
    tests.push_back({ "Empty string with ws separator","","", &wsSeparators });
    tests.push_back({ "Empty string with path separator","","", &pathSeparators });
    tests.push_back({ "Empty string with line end separator","","", &lineEndSeparators });

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << " : " << test.name << ": Input = \""
            << test.inVal << "\", ";
        std::string resultStr{test.seps ? FirstToken(test.inVal, *(test.seps)) : FirstToken(test.inVal)};
        result = (test.expected.compare(resultStr) == 0);
        std::cerr << "Got = \"" << resultStr << "\" -> "
            << PassFail(result);
        if (!result)
            std::cerr << ", expected \"" << test.expected << "\"";
        std::cerr << std::endl;

        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestLastToken()
{
    using NaM::CppScratch::LastToken;
    TestRunCerr run("Get last token from a string");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestSplitFirstToken()
{
    using NaM::CppScratch::SplitFirstToken;
    TestRunCerr run("Split a string on the first token");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestSplitLastToken()
{
    using NaM::CppScratch::SplitLastToken;
    TestRunCerr run("Split a string on the last token");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestPath_BaseName()
{
    using NaM::CppScratch::Path::BaseName;
    TestRunCerr run("Get the basename from a path (filename + extension)");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestPath_DirName()
{
    using NaM::CppScratch::Path::DirName;
    TestRunCerr run("Get the dirname from a path (directory/folder name)");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestPath_FileShortName()
{
    TestRunCerr run("Get the filenamename from a path (filename - extension)");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestPath_AppendDelimiter()
{
    TestRunCerr run("Append a path delimiter to the end of a string");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}

TestGroupResult TestPath_StripDelimiter()
{
    TestRunCerr run("Remove a path delimiter from the end of a string");

    struct TestVal
    {
        std::string name;
    };

    std::list<TestVal> tests;
    bool result;
    TestGroupResult runResult;

    size_t passCount{ 0 }, failCount{ 0 }, testNum{ 1 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << "TODO"
            << PassFail(result) << std::endl;
        runResult &= result;
        if (result)
            ++passCount;
        else
            ++failCount;
    }

    return runResult;
}
