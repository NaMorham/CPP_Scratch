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
    inline TestGroupResult& operator&=(const bool result)
    {
        if (result) { ++numPassed; }
        return *this;
    }
    inline TestGroupResult& operator+=(const TestGroupResult& other)
    {
        numPassed += other.numPassed;
        numTests += other.numTests;
        return *this;
    }
};

std::ostream& operator<<(std::ostream& oss, const TestGroupResult& res);

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

/* Colour vals */
#ifndef DONTUSE_COLOURXX
constexpr const char* const def_C_NRM{"\033[0m"};

constexpr const char* const def_C_BLD{"\033[1m"};
constexpr const char* const def_C_ITL{"\033[3m"};
constexpr const char* const def_C_UND{"\033[4m"};

constexpr const char* const def_C_BLK{"\033[30m"};
constexpr const char* const def_C_RED{"\033[31m"};
constexpr const char* const def_C_GRN{"\033[32m"};
constexpr const char* const def_C_YEL{"\033[33m"};
constexpr const char* const def_C_BLU{"\033[34m"};
constexpr const char* const def_C_MAG{"\033[35m"};
constexpr const char* const def_C_CYN{"\033[36m"};
constexpr const char* const def_C_WHT{"\033[37m"};

constexpr const char* const def_C_BBLK{"\033[90m"};
constexpr const char* const def_C_BRED{"\033[91m"};
constexpr const char* const def_C_BGRN{"\033[92m"};
constexpr const char* const def_C_BYEL{"\033[93m"};
constexpr const char* const def_C_BBLU{"\033[94m"};
constexpr const char* const def_C_BMAG{"\033[95m"};
constexpr const char* const def_C_BCYN{"\033[96m"};
constexpr const char* const def_C_BWHT{"\033[97m"};

constexpr const char* const def_CB_BLK{"\033[40m"};
constexpr const char* const def_CB_RED{"\033[41m"};
constexpr const char* const def_CB_GRN{"\033[42m"};
constexpr const char* const def_CB_YEL{"\033[43m"};
constexpr const char* const def_CB_BLU{"\033[44m"};
constexpr const char* const def_CB_MAG{"\033[45m"};
constexpr const char* const def_CB_CYN{"\033[46m"};
constexpr const char* const def_CB_WHT{"\033[47m"};

constexpr const char* const def_CB_BBLK{"\033[100m"};
constexpr const char* const def_CB_BRED{"\033[101m"};
constexpr const char* const def_CB_BGRN{"\033[102m"};
constexpr const char* const def_CB_BYEL{"\033[103m"};
constexpr const char* const def_CB_BBLU{"\033[104m"};
constexpr const char* const def_CB_BMAG{"\033[105m"};
constexpr const char* const def_CB_BCYN{"\033[106m"};
constexpr const char* const def_CB_BWHT{"\033[107m"};
#else
constexpr const char* const def_C_NRM{""};

constexpr const char* const def_C_BLD{""};
constexpr const char* const def_C_ITL{""};
constexpr const char* const def_C_UND{""};

constexpr const char* const def_C_BLK{""};
constexpr const char* const def_C_RED{""};
constexpr const char* const def_C_GRN{""};
constexpr const char* const def_C_YEL{""};
constexpr const char* const def_C_BLU{""};
constexpr const char* const def_C_MAG{""};
constexpr const char* const def_C_CYN{""};
constexpr const char* const def_C_WHT{""};

constexpr const char* const def_C_BBLK{""};
constexpr const char* const def_C_BRED{""};
constexpr const char* const def_C_BGRN{""};
constexpr const char* const def_C_BYEL{""};
constexpr const char* const def_C_BBLU{""};
constexpr const char* const def_C_BMAG{""};
constexpr const char* const def_C_BCYN{""};
constexpr const char* const def_C_BWHT{""};

constexpr const char* const def_CB_BLK{""};
constexpr const char* const def_CB_RED{""};
constexpr const char* const def_CB_GRN{""};
constexpr const char* const def_CB_YEL{""};
constexpr const char* const def_CB_BLU{""};
constexpr const char* const def_CB_MAG{""};
constexpr const char* const def_CB_CYN{""};
constexpr const char* const def_CB_WHT{""};

constexpr const char* const def_CB_BBLK{""};
constexpr const char* const def_CB_BRED{""};
constexpr const char* const def_CB_BGRN{""};
constexpr const char* const def_CB_BYEL{""};
constexpr const char* const def_CB_BBLU{""};
constexpr const char* const def_CB_BMAG{""};
constexpr const char* const def_CB_BCYN{""};
constexpr const char* const def_CB_BWHT{""};
#endif

#if 1
constexpr const char* const udef_C_NRM{ "" };

constexpr const char* const udef_C_BLD{ "" };
constexpr const char* const udef_C_ITL{ "" };
constexpr const char* const udef_C_UND{ "" };

constexpr const char* const udef_C_BLK{ "" };
constexpr const char* const udef_C_RED{ "" };
constexpr const char* const udef_C_GRN{ "" };
constexpr const char* const udef_C_YEL{ "" };
constexpr const char* const udef_C_BLU{ "" };
constexpr const char* const udef_C_MAG{ "" };
constexpr const char* const udef_C_CYN{ "" };
constexpr const char* const udef_C_WHT{ "" };

constexpr const char* const udef_C_BBLK{ "" };
constexpr const char* const udef_C_BRED{ "" };
constexpr const char* const udef_C_BGRN{ "" };
constexpr const char* const udef_C_BYEL{ "" };
constexpr const char* const udef_C_BBLU{ "" };
constexpr const char* const udef_C_BMAG{ "" };
constexpr const char* const udef_C_BCYN{ "" };
constexpr const char* const udef_C_BWHT{ "" };

constexpr const char* const udef_CB_BLK{ "" };
constexpr const char* const udef_CB_RED{ "" };
constexpr const char* const udef_CB_GRN{ "" };
constexpr const char* const udef_CB_YEL{ "" };
constexpr const char* const udef_CB_BLU{ "" };
constexpr const char* const udef_CB_MAG{ "" };
constexpr const char* const udef_CB_CYN{ "" };
constexpr const char* const udef_CB_WHT{ "" };

constexpr const char* const udef_CB_BBLK{ "" };
constexpr const char* const udef_CB_BRED{ "" };
constexpr const char* const udef_CB_BGRN{ "" };
constexpr const char* const udef_CB_BYEL{ "" };
constexpr const char* const udef_CB_BBLU{ "" };
constexpr const char* const udef_CB_BMAG{ "" };
constexpr const char* const udef_CB_BCYN{ "" };
constexpr const char* const udef_CB_BWHT{ "" };
#endif

#if 1
static std::string C_NRM{ def_C_NRM };

static std::string C_BLD{ def_C_BLD };
static std::string C_ITL{ def_C_ITL };
static std::string C_UND{ def_C_UND };

static std::string C_BLK{ def_C_BLK };
static std::string C_RED{ def_C_RED };
static std::string C_GRN{ def_C_GRN };
static std::string C_YEL{ def_C_YEL };
static std::string C_BLU{ def_C_BLU };
static std::string C_MAG{ def_C_MAG };
static std::string C_CYN{ def_C_CYN };
static std::string C_WHT{ def_C_WHT };

static std::string C_BBLK{ def_C_BBLK };
static std::string C_BRED{ def_C_BRED };
static std::string C_BGRN{ def_C_BGRN };
static std::string C_BYEL{ def_C_BYEL };
static std::string C_BBLU{ def_C_BBLU };
static std::string C_BMAG{ def_C_BMAG };
static std::string C_BCYN{ def_C_BCYN };
static std::string C_BWHT{ def_C_BWHT };

static std::string CB_BLK{ def_CB_BLK };
static std::string CB_RED{ def_CB_RED };
static std::string CB_GRN{ def_CB_GRN };
static std::string CB_YEL{ def_CB_YEL };
static std::string CB_BLU{ def_CB_BLU };
static std::string CB_MAG{ def_CB_MAG };
static std::string CB_CYN{ def_CB_CYN };
static std::string CB_WHT{ def_CB_WHT };

static std::string CB_BBLK{ def_CB_BBLK };
static std::string CB_BRED{ def_CB_BRED };
static std::string CB_BGRN{ def_CB_BGRN };
static std::string CB_BYEL{ def_CB_BYEL };
static std::string CB_BBLU{ def_CB_BBLU };
static std::string CB_BMAG{ def_CB_BMAG };
static std::string CB_BCYN{ def_CB_BCYN };
static std::string CB_BWHT{ def_CB_BWHT };
#endif

const std::string PassFail(const bool result);
void UseColour(const bool use);
const bool isWinCmdExe();

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    std::cout << FileShortName(argv[0])
        << " Version " << TestUtilsTests_VersionFull << std::endl << std::endl;

    UseColour(!isWinCmdExe() || true);

    TestGroupResult allResult;
    allResult += TestTrueOrFalse();
    allResult += TestXofYStr();
    allResult += TestTextNumLength();
    allResult += TestPtrString();
    allResult += TestFirstToken();
    allResult += TestLastToken();
    allResult += TestSplitFirstToken();
    allResult += TestSplitLastToken();
    allResult += TestPath_BaseName();
    allResult += TestPath_DirName();
    allResult += TestPath_FileShortName();
    allResult += TestPath_AppendDelimiter();
    allResult += TestPath_StripDelimiter();

    std::cout << std::endl << "After all tests. " << allResult << std::endl << std::endl;
    return allResult.Passed() ? EXIT_SUCCESS : EXIT_FAILURE;
}

//-----------------------------------------------------------------------------
const std::string PassFail(const bool result)
{
    static constexpr const char* const _passStr{ "PASS" };
    static constexpr const char* const _failStr{ "FAIL" };

    std::stringstream ss;
    if (result)
    {
        ss << C_BLD << C_GRN << _passStr << C_NRM;
    }
    else
    {
        ss << C_BLD << C_RED << _failStr << C_NRM;
    }
    return ss.str();
}

std::string TestNumLabel(size_t x, size_t y)
{
    std::stringstream ss;
    ss << "Test [" << XofYStr(x, y) << "]";
    return ss.str();
}

std::string EscapeString(const std::string& str)
{
    std::string s;
    s.reserve(str.size() * 2 + 1);
    for (size_t idx = 0, oIdx = 0; idx < str.size(); ++idx)
    {
        char c{ str.at(idx) };
        switch (c)
        {
        case '\t': s.append("\\t"); break;
        case '\r': s.append("\\r"); break;
        case '\n': s.append("\\n"); break;
        case '\b': s.append("\\b"); break;
        case '\v': s.append("\\v"); break;
        case '\a': s.append("\\a"); break;
        default:
            s.push_back(c);
        }
        s.shrink_to_fit();
    }
    return s;
}

std::string LimitLen(const std::string& str, const size_t& maxLen)
{
    if ((str.size() < maxLen) || (maxLen < 7))
    {
        return str;
    }
    else
    {
        size_t keepSz{ static_cast<size_t>(std::floor(maxLen / 2) - 5) };
        std::string s{ str.substr(0, keepSz) };
        s.append(" ... ").append(str.substr(str.size() - (keepSz + 5)));
        return s;
    }
}

std::ostream& operator<<(std::ostream& oss, const TestGroupResult& res)
{
    bool allPassed{ res.Passed() };
    std::stringstream colPrefix;
    if (res.numTests == 0)
        colPrefix << C_YEL;
    else if (allPassed)
        colPrefix << C_BLD << C_GRN;
    else
        colPrefix << C_BLD << C_RED;
    std::string& colNorm{ C_NRM };
    float pct{ res.numTests ? (res.numPassed / res.numTests * 100.0f) : 0.0f };
    oss << colPrefix.str() << res.numPassed << colNorm << " of "
        << colPrefix.str() << res.numTests << colNorm << " tests passed ("
        << colPrefix.str() << std::fixed << std::setprecision(2) << pct << "%"
        << colNorm << ")";
    return oss;
}

struct FuncColPrefix_t
{
    std::string prefix;
    FuncColPrefix_t() {
        if (prefix.empty())
        {
            std::stringstream ss;
            ss << C_BLD << C_BLU;
            prefix.assign(ss.str());
        }
    }
    const std::string& operator()() { return prefix; }
};
static FuncColPrefix_t _FuncColPrefix;
static const std::string FuncColPrefix{ _FuncColPrefix.prefix };
static const std::string FuncColSuffix{ C_NRM };

TestGroupResult TestTrueOrFalse()
{
    TestRunCerr run("Test bool to string : TrueOrFalse(...)");
    std::string got;
    bool result{ true };
    TestGroupResult runResult{ 0, 2 };

    got.assign(TrueOrFalse(true));
    result = (got.compare("true") == 0);
    runResult &= result;
    std::cerr << g_counter << "Test [1/2]: "
        << FuncColPrefix << "TrueOrFalse" << FuncColSuffix << "(true)"
        << " -> \"" << got << "\" == "
        << PassFail(result) << std::endl;

    got.assign(TrueOrFalse(false));
    result = (got.compare("false") == 0);
    runResult &= result;
    std::cerr << g_counter << "Test [2/2]: "
        << FuncColPrefix << "TrueOrFalse" << FuncColSuffix << "(false)"
        << " -> \"" << got << "\" == "
        << PassFail(result) << std::endl;

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

TestGroupResult TestTextNumLength()
{
    TestRunCerr run("Get text length of number");

    struct TestVal
    {
        std::string name;
        int32_t inVal;
        size_t expected;
    };

    std::list<TestVal> tests;
    bool result{ true };
    TestGroupResult runResult;

    tests.push_back({ "Zero", 0, 1 });
    tests.push_back({ "One", 1, 1 });
    tests.push_back({ "Minus one", -1, 2 });
    tests.push_back({ "Double digit", 42, 2 });
    tests.push_back({ "Negative double digit", -56, 3 });
    tests.push_back({ "Triple digits", 123, 3 });
    tests.push_back({ "Negative triple digits", -998, 4 });

    for (size_t i = 0; i < 10; ++i)
    {
        std::stringstream ss;
        ss << "Test positive (10 ^ " << i << ")";
        tests.push_back({ ss.str(), static_cast<int32_t>(std::pow(10, i)), i + 1 });
        ss.clear();
    }
    for (size_t i = 0; i < 10; ++i)
    {
        std::stringstream ss;
        ss << "Test negative (10 ^ " << i << ")";
        tests.push_back({ ss.str(), -static_cast<int32_t>(std::pow(10, i)), i + 2 });
        ss.clear();
    }

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, resultVal;
    for (const TestVal test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": ";
        resultVal = TextNumLength(test.inVal);
        result = (resultVal == test.expected);
        std::cerr << test.name << ": "
            << std::endl << "\t" << FuncColPrefix << "TextNumLength" << FuncColSuffix
            << "(" << test.inVal << ") = "
            << resultVal << " -> " << PassFail(result);
        if (!result)
            std::cerr << ", expected \"" << test.expected << "\"";
        std::cerr << std::endl;
        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
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
    bool result{ true };
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

    runResult.numTests = tests.size();
    size_t testNum{ 1 };
    for (TestVal& test : tests)
    {
        if (test.sep.empty())
            got.assign(XofYStr(test.x, test.y));
        else
            got.assign(XofYStr(test.x, test.y, test.sep));
        result = (test.expected.compare(got) == 0);
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "XofYStr" << FuncColSuffix
            << "(" << test.x << ", " << test.y << ") = \"" << got << "\""
            << " -> " << PassFail(result);
        if (!result)
            std::cerr << ", expected \"" << test.expected << "\"";
        std::cerr << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

TestGroupResult TestPtrString()
{
    TestRunCerr run("Pointer to string");

    struct TestVal
    {
        std::string name;
        const void* const inPtr;
    };

    bool result{ true };
    TestGroupResult runResult;
    std::list<TestVal> tests;
    size_t testNum{ 1 };

    int8_t i8{ 0 };
    int16_t i16{ 0 };
    int32_t i32{ 0 };
    int64_t i64{ 0 };

    uint8_t ui8{ 0 };
    uint16_t ui16{ 0 };
    uint32_t ui32{ 0 };
    uint64_t ui64{ 0 };

    tests.push_back({ "Null ptr" , nullptr});
    tests.push_back({ "NULL" , NULL });
    tests.push_back({ "int8_t", &i8 });
    tests.push_back({ "int16_t", &i16 });
    tests.push_back({ "int32_t", &i32 });
    tests.push_back({ "int64_t", &i64 });
    tests.push_back({ "uint8_t", &ui8 });
    tests.push_back({ "uint16_t", &ui16 });
    tests.push_back({ "uint32_t", &ui32 });
    tests.push_back({ "uint64_t", &ui64 });

    runResult.numTests = tests.size();
    for (const TestVal& test : tests)
    {
        // there is no pass fail here since the different OS' seem to provide a different string
        // we could calculate for the architecture, but it seems like a hassle
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size())
            << ": " << test.name << ": "
            << std::endl << "\t" << FuncColPrefix << "PtrString" << FuncColSuffix
            << "(...) = \"" << PtrString(test.inPtr) << "\""
            << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
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
        bool seps;
        const std::string separator;

        TestVal() = delete;
        TestVal(const std::string& n, const std::string& i, const std::string& e)
            : name{ n }, inVal{ i }, expected{ e }, seps{ false }, separator{ "" } {}
        TestVal(const std::string& n, const std::string& i, const std::string& e, const std::string& s)
            : name{ n }, inVal{ i }, expected{ e }, seps{ true }, separator{ s } {}
    };

    std::list<TestVal> tests;
    bool result{ true };
    std::string sepStr;
    TestGroupResult runResult;
    static const std::string single{"ALongStringWithNoBreak"};
    static const std::string fox{ "The quick brown fox jumps over the lazy dog" };
    static const std::string para{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis\n"
        "sed."};
    static const std::string paraL1{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor" };

    // remember it defaults to ws separator
    tests.push_back({ "Empty string", "", "" });
    tests.push_back({ "Empty string with ws separator", "", "", wsSeparators });
    tests.push_back({ "Empty string with path separator", "", "", pathSeparators });
    tests.push_back({ "Empty string with line end separator", "", "", lineEndSeparators });

    tests.push_back({ "Single token", single, single });
    tests.push_back({ "Single token with ws separator", single, single, wsSeparators });
    tests.push_back({ "Single token with found custom separator", single, "ALong", "S" });
    tests.push_back({ "Single token with no found custom separator", single, single, "~" });

    tests.push_back({ "Sentence", fox, "The" });
    tests.push_back({ "Sentence with ws separator", fox, "The", wsSeparators });

    tests.push_back({ "Paragraph", para, "Lorem" });
    tests.push_back({ "Paragraph with path separator", para, para, pathSeparators });
    tests.push_back({ "Paragraph with line end separator", para, paraL1, lineEndSeparators });

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::string resultStr{test.seps ? FirstToken(test.inVal, test.separator) : FirstToken(test.inVal)};
        result = (test.expected.compare(resultStr) == 0);
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "FirstToken" << FuncColSuffix
            << "(\"" << LimitLen(EscapeString(test.inVal), maxStrShowLen) << "\"";
        if (test.seps)
        {
            std::cerr << ", \"" << EscapeString(test.separator) << "\")";
        }
        else
        {
            std::cerr << ")";
        }
        std::cerr << " = \""
            << LimitLen(EscapeString(resultStr), maxStrShowLen)
            << "\" -> " << PassFail(result);
        if (!result)
        {
            std::cerr << ", expected \""
                << LimitLen(EscapeString(test.expected), maxStrShowLen) << "\"";
        }
        std::cerr << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

TestGroupResult TestLastToken()
{
    using NaM::CppScratch::LastToken;
    TestRunCerr run("Get last token from a string");

    struct TestVal
    {
        std::string name;
        std::string inVal;
        std::string expected;
        bool seps;
        const std::string separator;

        TestVal() = delete;
        TestVal(const std::string& n, const std::string& i, const std::string& e)
            : name{ n }, inVal{ i }, expected{ e }, seps{ false }, separator{ "" } {}
        TestVal(const std::string& n, const std::string& i, const std::string& e, const std::string& s)
            : name{ n }, inVal{ i }, expected{ e }, seps{ true }, separator{ s } {}
    };

    std::list<TestVal> tests;
    bool result{ true };
    std::string sepStr;
    TestGroupResult runResult;
    static const std::string single{"ALongStringWithNoBreak"};
    static const std::string fox{ "The quick brown fox jumps over the lazy dog" };
    static const std::string para{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis\n"
        "sed."};
    static const std::string paraL6{ "sed." };

    // remember it defaults to ws separator
    tests.push_back({ "Empty string", "", "" });
    tests.push_back({ "Empty string with ws separator", "", "", wsSeparators });
    tests.push_back({ "Empty string with path separator", "", "", pathSeparators });
    tests.push_back({ "Empty string with line end separator", "", "", lineEndSeparators });

    tests.push_back({ "Single token", single, single });
    tests.push_back({ "Single token with ws separator", single, single, wsSeparators });
    tests.push_back({ "Single token with found custom separator", single, "tringWithNoBreak", "S" });
    tests.push_back({ "Single token with no found custom separator", single, single, "~" });

    tests.push_back({ "Sentence", fox, "dog" });
    tests.push_back({ "Sentence with ws separator", fox, "dog", wsSeparators });

    tests.push_back({ "Paragraph", para, "facilisis\nsed." });
    tests.push_back({ "Paragraph with path separator", para, para, pathSeparators });
    tests.push_back({ "Paragraph with line end separator", para, paraL6, lineEndSeparators });

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::string resultStr{test.seps ? LastToken(test.inVal, test.separator) : LastToken(test.inVal)};
        result = (test.expected.compare(resultStr) == 0);
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "LastToken" << FuncColSuffix
            << "(\"" << LimitLen(EscapeString(test.inVal), maxStrShowLen) << "\"";
        if (test.seps)
        {
            std::cerr << ", \"" << EscapeString(test.separator) << "\")";
        }
        else
        {
            std::cerr << ")";
        }
        std::cerr << " = \""
            << LimitLen(EscapeString(resultStr), maxStrShowLen)
            << "\" -> " << PassFail(result);
        if (!result)
        {
            std::cerr << ", expected \""
                << LimitLen(EscapeString(test.expected), maxStrShowLen) << "\"";
        }
        std::cerr << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

TestGroupResult TestSplitFirstToken()
{
    using NaM::CppScratch::SplitFirstToken;
    TestRunCerr run("Split a string on the first token");

    struct TestVal
    {
        const std::string name;
        const std::string inVal;
        const std::string expectedFirst;
        const std::string expectedRemainder;
        bool seps;
        const std::string separator;

        TestVal() = delete;
        TestVal(const std::string& n, const std::string& i,
            const std::string& e, const std::string& r)
            : name{ n }, inVal{ i }, expectedFirst{ e }, expectedRemainder{ r },
              seps{ false }, separator{ "" } {}
        TestVal(const std::string& n, const std::string& i,
            const std::string& e, const std::string& r, const std::string& s)
            : name{ n }, inVal{ i }, expectedFirst{ e }, expectedRemainder{ r },
              seps{ true }, separator{ s } {}
    };

    std::list<TestVal> tests;
    bool result{ true };
    TestGroupResult runResult;
    static const std::string single{"ALongStringWithNoBreak"};
    static const std::string fox{ "The quick brown fox jumps over the lazy dog" };
    static const std::string foxRem{ "quick brown fox jumps over the lazy dog" };
    static const std::string para{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis\n"
        "sed."};
    static const std::string paraL1{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor" };
    static const std::string paraL2_6{
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis\n"
        "sed."};
    static const std::string paraWSRem{
        "ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis\n"
        "sed."};

    tests.push_back({ "Empty", "", "", "" });
    tests.push_back({ "Empty string with ws separator", "", "", "", wsSeparators});
    tests.push_back({ "Empty string with path separator", "", "", "", pathSeparators});
    tests.push_back({ "Empty string with line end separator", "", "", "", lineEndSeparators});

    tests.push_back({ "Single token", single, single, "" });
    tests.push_back({ "Single token with ws separator", single, single, "", wsSeparators});
    tests.push_back({ "Single token with found custom separator", single, "ALong", "tringWithNoBreak", "S" });
    tests.push_back({ "Single token with no found custom separator", single, single, "", "~" });

    tests.push_back({ "Sentence", fox, "The", foxRem });
    tests.push_back({ "Sentence with ws separator", fox, "The", foxRem, wsSeparators });

    tests.push_back({ "Paragraph", para, "Lorem", paraWSRem });
    tests.push_back({ "Paragraph with path separator", para, para, "", pathSeparators });
    tests.push_back({ "Paragraph with line end separator", para, paraL1, paraL2_6, lineEndSeparators });

    std::string t, r;
    t = SplitFirstToken(single, r, wsSeparators);
    t = SplitFirstToken(single, r);

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::string token, remainder;
        std::cerr << g_counter
            << TestNumLabel(testNum++, tests.size()) << ": " << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "SplitFirstToken" << FuncColSuffix
            << "(\"" << LimitLen(EscapeString(test.inVal), maxStrShowLen)
            << "\", remainder&)" << std::endl
            << "\t  = {\"";

        if (test.seps)
        {
            token.assign(SplitFirstToken(test.inVal, remainder, test.separator));
        }
        else
        {
            token.assign(SplitFirstToken(test.inVal, remainder));
        }
        result = ((test.expectedFirst.compare(token) == 0) &&
                (test.expectedRemainder.compare(remainder) == 0));

        std::cerr << LimitLen(EscapeString(token), maxStrShowLen)
            << "\", \"" << LimitLen(EscapeString(remainder), maxStrShowLen)
            << "\"} -> " << PassFail(result);
        if (!result)
        {
            std::cerr << ", " << std::endl << "\texpected {\""
                << LimitLen(EscapeString(test.expectedFirst), maxStrShowLen)
                << "\", \""
                << LimitLen(EscapeString(test.expectedRemainder), maxStrShowLen)
                << "\"}";
        }
        std::cerr << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

TestGroupResult TestSplitLastToken()
{
    using NaM::CppScratch::SplitLastToken;
    TestRunCerr run("Split a string on the last token");

    struct TestVal
    {
        const std::string name;
        const std::string inVal;
        const std::string expectedLast;
        const std::string expectedRemainder;
        bool seps;
        const std::string separator;

        TestVal() = delete;
        TestVal(const std::string& n, const std::string& i,
            const std::string& e, const std::string& r)
            : name{ n }, inVal{ i }, expectedLast{ e }, expectedRemainder{ r },
            seps{ false }, separator{ "" } {}
        TestVal(const std::string& n, const std::string& i,
            const std::string& e, const std::string& r, const std::string& s)
            : name{ n }, inVal{ i }, expectedLast{ e }, expectedRemainder{ r },
            seps{ true }, separator{ s } {}
    };

    std::list<TestVal> tests;
    bool result{ true };
    TestGroupResult runResult;

    static const std::string single{"ALongStringWithNoBreak"};
    static const std::string fox{ "The quick brown fox jumps over the lazy dog" };
    static const std::string foxRem{ "The quick brown fox jumps over the lazy" };
    static const std::string para{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis\n"
        "sed."};
    static const std::string paraL1_5{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu facilisis"};
    static const std::string paraL6{ "sed." };
    static const std::string paraWSRem{
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna aliqua.Vel pharetra vel turpis nunc eget\n"
        "lorem.Maecenas ultricies mi eget mauris pharetra et ultrices.Id velit ut\n"
        "tortor pretium viverra suspendisse potenti nullam.Consectetur adipiscing elit\n"
        "ut aliquam purus sit amet.Morbi leo urna molestie at elementum eu"};

    // remember it defaults to ws separator
    tests.push_back({ "Empty string", "", "", ""});
    tests.push_back({ "Empty string with ws separator", "", "", "", wsSeparators});
    tests.push_back({ "Empty string with path separator", "", "", "", pathSeparators});
    tests.push_back({ "Empty string with line end separator", "", "", "", lineEndSeparators});

    tests.push_back({ "Single token", single, single, "" });
    tests.push_back({ "Single token with ws separator", single, single, "", wsSeparators});
    tests.push_back({ "Single token with found custom separator", single, "tringWithNoBreak", "ALong", "S" });
    tests.push_back({ "Single token with no found custom separator", single, single, "", "~" });

    tests.push_back({ "Sentence", fox, "dog", foxRem });
    tests.push_back({ "Sentence with ws separator", fox, "dog", foxRem, wsSeparators });

    tests.push_back({ "Paragraph", para, "facilisis\nsed.", paraWSRem });
    tests.push_back({ "Paragraph with path separator", para, para, "", pathSeparators});
    tests.push_back({ "Paragraph with line end separator", para, paraL6, paraL1_5, lineEndSeparators });

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::string token, remainder;
        std::cerr << g_counter
            << TestNumLabel(testNum++, tests.size()) << ": " << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "SplitLastToken" << FuncColSuffix
            << "(\"" << LimitLen(EscapeString(test.inVal), maxStrShowLen)
            << "\", remainder&)" << std::endl
            << "\t  = {\"";
        if (test.seps)
        {
            token.assign(SplitLastToken(test.inVal, remainder, test.separator));
        }
        else
        {
            token.assign(SplitLastToken(test.inVal, remainder));
        }
        result = ((test.expectedLast.compare(token) == 0) &&
            (test.expectedRemainder.compare(remainder) == 0));

        std::cerr << LimitLen(EscapeString(token), maxStrShowLen)
            << "\", \"" << LimitLen(EscapeString(remainder), maxStrShowLen)
            << "\"} -> " << PassFail(result);
        if (!result)
        {
            std::cerr << ", " << std::endl << "\texpected {\""
                << LimitLen(EscapeString(test.expectedLast), maxStrShowLen)
                << "\", \""
                << LimitLen(EscapeString(test.expectedRemainder), maxStrShowLen)
                << "\"}";
        }
        std::cerr << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

TestGroupResult TestPath_BaseName()
{
    using NaM::CppScratch::Path::BaseName;
    TestRunCerr run("Get the basename from a path (filename + extension)");

    struct TestVal
    {
        const std::string name;
        const std::string inPath;
        const std::string expected;
        bool sep;
        const std::string separator;

        TestVal() = delete;
        TestVal(const std::string& n, const std::string& i, const std::string& e)
            : name{ n }, inPath{ i }, expected{ e }, sep{ false }, separator{ "" } {}
        TestVal(const std::string& n, const std::string& i,
                const std::string& e, const std::string& s)
            : name{ n }, inPath{ i }, expected{ e }, sep{ true }, separator{ s } {}
    };

    std::list<TestVal> tests;
    bool result{ true };
    TestGroupResult runResult;
    const std::string winPath{ "C:\\Program Files\\Foo\\Bar\\someFile.txt" };
    const std::string winPathRes{ "someFile.txt" };
    const std::string winPathNF{ "C:\\Program Files\\Nope\\NoFile\\" };
    const std::string winPathNFRes{ "" };
    const std::string winPathNFD{ "C:\\Program Files\\Nope\\NoFile" };
    const std::string winPathNFDRes{ "NoFile" }; // as far as the code is concerend this ends in a filename

    tests.push_back({ "Empty", "", "" });
    tests.push_back({ "Empty with separator", "", "", pathSeparators });
    tests.push_back({ "Empty with Windows separator", "", "", "\\"});
    tests.push_back({ "Empty with Mac separator", "", "", ":" });
    tests.push_back({ "Empty with posix separator", "", "", "/" });
    tests.push_back({ "Empty with custom separator", "", "", "@" });

    tests.push_back({ "Windows path", winPath, winPathRes });
    tests.push_back({ "Windows path with separator", winPath, winPathRes, pathSeparators });
    tests.push_back({ "Windows path with Windows separator", winPath, winPathRes, "\\" });
    tests.push_back({ "Windows path with Mac separator", winPath, winPath.substr(2), ":"});
    tests.push_back({ "Windows path with posix separator", winPath, winPath, "/" }); // no match, so it is all the basename
    tests.push_back({ "Windows path with custom separator", winPath, winPath, "@" });

    tests.push_back({ "Windows path no file", winPathNF, winPathNFRes });
    tests.push_back({ "Windows path no file with separator", winPathNF, winPathNFRes, pathSeparators });
    tests.push_back({ "Windows path no file with Windows separator", winPathNF, winPathNFRes, "\\" });
    tests.push_back({ "Windows path no file with Mac separator", winPathNF, winPathNF.substr(2), ":" });
    tests.push_back({ "Windows path no file with posix separator", winPathNF, winPathNF, "/" }); // no match, so it is all the basename
    tests.push_back({ "Windows path no file with custom separator", winPathNF, winPathNF, "@" });

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "BaseName" << FuncColSuffix
            << "(\"" << LimitLen(EscapeString(test.inPath), maxStrShowLen) << "\"";
        std::string baseName;
        if (test.sep)
        {
            std::cerr << ", \"" << EscapeString(test.separator) << "\")";
            baseName.assign(BaseName(test.inPath, test.separator));
        }
        else
        {
            std::cerr << ")";
            baseName.assign(BaseName(test.inPath));
        }
        result = (test.expected.compare(baseName) == 0);
        std::cerr << " = \"" << LimitLen(EscapeString(baseName), maxStrShowLen) << "\"";
        std::cerr << " -> " << PassFail(result);

        if (!result)
        {
            std::cerr << ", expected \""
                << LimitLen(EscapeString(test.expected), maxStrShowLen) << "\"";
        }
        std::cerr << std::endl;

        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
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
    bool result{ true };
    TestGroupResult runResult;

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "TODO" << FuncColSuffix 
            << PassFail(result) << std::endl;
        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
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
    bool result{ true };
    TestGroupResult runResult;

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "TODO" << FuncColSuffix
            << PassFail(result) << std::endl;
        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
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
    bool result{ true };
    TestGroupResult runResult;

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "TODO" << FuncColSuffix
            << PassFail(result) << std::endl;
        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
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
    bool result{ true };
    TestGroupResult runResult;

    runResult.numTests = tests.size();
    size_t testNum{ 1 }, maxStrShowLen{ 35 };
    for (TestVal& test : tests)
    {
        std::cerr << g_counter << TestNumLabel(testNum++, tests.size()) << ": "
            << test.name << ": " << std::endl
            << "\t" << FuncColPrefix << "TODO" << FuncColSuffix
            << PassFail(result) << std::endl;
        runResult &= result;
    }

    std::cout << std::endl << C_BLD << "RUN:" << C_NRM << "\"" << run.Name() << "\": " << runResult << std::endl;
    return runResult;
}

const bool isWinCmdExe()
{
#ifdef _MSC_VER
    char* buf;  // [LARGE_STR_BUF + 1];
    size_t requiredSize;

    getenv_s(&requiredSize, NULL, 0, "ComSpec");

    buf = new char[requiredSize + 1];
    memset(buf, 0, sizeof(char) * (requiredSize + 1));

    // ComSpec=C:\Windows\system32\cmd.exe
    getenv_s(&requiredSize, buf, requiredSize, "ComSpec");
    std::cerr << "Got comspec: \"" << buf << "\"" << std::endl;

    if (strstr(buf, "cmd.exe"))
    {
        delete[] buf;
        return true;
    }
    else if (strstr(buf, "command.com"))
    {
        delete[] buf;
        return true;
    }
    else
    {
        delete[] buf;
        return false;
    }

#else
    std::cerr << "Not MSVS called" << std::endl;
    return false;
#endif
}

void UseColour(const bool use)
{
    C_NRM.assign(use ? def_C_NRM : udef_C_NRM);

    C_BLD.assign(use ? def_C_BLD : udef_C_BLD);
    C_ITL.assign(use ? def_C_ITL : udef_C_ITL);
    C_UND.assign(use ? def_C_UND : udef_C_UND);

    C_BLK.assign(use ? def_C_BLK : udef_C_BLK);
    C_RED.assign(use ? def_C_RED : udef_C_RED);
    C_GRN.assign(use ? def_C_GRN : udef_C_GRN);
    C_YEL.assign(use ? def_C_YEL : udef_C_YEL);
    C_BLU.assign(use ? def_C_BLU : udef_C_BLU);
    C_MAG.assign(use ? def_C_MAG : udef_C_MAG);
    C_CYN.assign(use ? def_C_CYN : udef_C_CYN);
    C_WHT.assign(use ? def_C_WHT : udef_C_WHT);

    C_BBLK.assign(use ? def_C_BBLK : udef_C_BBLK);
    C_BRED.assign(use ? def_C_BRED : udef_C_BRED);
    C_BGRN.assign(use ? def_C_BGRN : udef_C_BGRN);
    C_BYEL.assign(use ? def_C_BYEL : udef_C_BYEL);
    C_BBLU.assign(use ? def_C_BBLU : udef_C_BBLU);
    C_BMAG.assign(use ? def_C_BMAG : udef_C_BMAG);
    C_BCYN.assign(use ? def_C_BCYN : udef_C_BCYN);
    C_BWHT.assign(use ? def_C_BWHT : udef_C_BWHT);

    CB_BLK.assign(use ? def_CB_BLK : udef_CB_BLK);
    CB_RED.assign(use ? def_CB_RED : udef_CB_RED);
    CB_GRN.assign(use ? def_CB_GRN : udef_CB_GRN);
    CB_YEL.assign(use ? def_CB_YEL : udef_CB_YEL);
    CB_BLU.assign(use ? def_CB_BLU : udef_CB_BLU);
    CB_MAG.assign(use ? def_CB_MAG : udef_CB_MAG);
    CB_CYN.assign(use ? def_CB_CYN : udef_CB_CYN);
    CB_WHT.assign(use ? def_CB_WHT : udef_CB_WHT);

    CB_BBLK.assign(use ? def_CB_BBLK : udef_CB_BBLK);
    CB_BRED.assign(use ? def_CB_BRED : udef_CB_BRED);
    CB_BGRN.assign(use ? def_CB_BGRN : udef_CB_BGRN);
    CB_BYEL.assign(use ? def_CB_BYEL : udef_CB_BYEL);
    CB_BBLU.assign(use ? def_CB_BBLU : udef_CB_BBLU);
    CB_BMAG.assign(use ? def_CB_BMAG : udef_CB_BMAG);
    CB_BCYN.assign(use ? def_CB_BCYN : udef_CB_BCYN);
    CB_BWHT.assign(use ? def_CB_BWHT : udef_CB_BWHT);
}
