#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "customStackVer.h"

namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        class StackWP;

        template<typename T>
        class StackWPNode;

        template<typename T>
        using StackWP_p = StackWP<T>*;

        template<typename T>
        using StackWPNode_p = StackWPNode<T>*;

        template<typename T>
        class TEST_StackWPNode
        {
        public:
            [[nodiscard]] static StackWPNode_p<T> NewNode(const T& data)
            {
                return new StackWPNode<T>(data);
            }
            static void DeleteNode(StackWPNode_p<T>& nodePtr)
            {
                if (nodePtr)
                {
                    delete nodePtr;
                    nodePtr = nullptr;
                }
            }
        };

        // TODO: Modify to use Identifiablee base class
        template<typename T>
        class StackWPNode
        {
        private:
            static std::uint64_t ms_ids;
            std::uint64_t m_nodeId;
            T m_data;
            StackWPNode_p<T> m_pNext;

            friend class NaM::CppScratch::TEST_StackWPNode<T>;

        protected:
            StackWPNode() = delete;
            StackWPNode(const T& data) : m_nodeId(ms_ids++), m_data(data), m_pNext(nullptr) {}
            StackWPNode(StackWPNode&& other) = delete;

            StackWPNode& operator=(const StackWPNode&) = delete;
            StackWPNode& operator=(StackWPNode&) = delete;

            virtual ~StackWPNode() {}

            [[nodiscard]] inline const StackWPNode* Next() const { return m_pNext; }
            [[nodiscard]] inline StackWPNode* Next() { return m_pNext; }

        public:
            [[nodiscard]] inline const std::uint64_t Id() const { return m_nodeId; }

            [[nodiscard]] inline const T& Data() const { return m_data; }
            [[nodiscard]] inline T& Data() { return m_data; }

            [[nodiscard]] inline const T& operator*() const { return Data(); }
            [[nodiscard]] inline T& operator*() { return Data(); }
        };

        template<typename T>
        class StackWP
        {
        private:
            static std::uint64_t ms_ids;
            std::uint64_t m_stackId;
            StackWPNode_p<T> m_pHead;
            size_t m_size;

        protected:
        public:
            StackWP() : m_stackId(ms_ids++), m_pHead(nullptr), m_size(0) {}
            virtual ~StackWP();

            [[nodiscard]] inline const std::uint64_t Id() const { return m_stackId; }

            [[nodiscard]] inline const StackWPNode_p<T> Head() const { return m_pHead; }
            [[nodiscard]] inline StackWPNode_p<T> Head() { return m_pHead; }
        };

        template<typename T>
        std::uint64_t StackWPNode<T>::ms_ids{0};

        template<typename T>
        std::uint64_t StackWP<T>::ms_ids{0};

    }  // end namespace CppScratch
}  // end namespace NaM

//------------------------------------------------------------------------
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
        std::string outString(in.substr(lastpos + 1));
        return outString;
    }
}

const std::string DASHES(std::string(70, '-'));
const std::string nullvalstr(std::string("0x").append(std::string(sizeof(nullptr_t), '0')));

const std::string& BoolStr(const bool& bVal)
{
    static std::string trueString{"true"};
    static std::string falseString{"false"};
    return (bVal ? trueString : falseString);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void DummyDataTests();
void StackWPNodeTests();
void StackWPTests();

int main(int argc, char* argv[])
{
    std::cout << LastToken(argv[0]) << " Version " << CustomStackWithPointers_VersionFull << std::endl;

    DummyDataTests();
    StackWPNodeTests();
    StackWPTests();

    std::cerr << std::endl << std::endl;
    return 0;
}

//------------------------------------------------------------------------
class _CounterVal
{
private:
    static std::uint64_t ms_count;
public:
    [[nodiscard]] inline const std::uint64_t GetCount() const { return ms_count; }
    [[nodiscard]] inline const std::uint64_t Count() const { return ++ms_count; }
};
std::uint64_t _CounterVal::ms_count = 0;
std::ostream& operator<<(std::ostream& oss, const _CounterVal& counter)
{
    oss << std::dec << std::setw(2) << counter.Count(); return oss;
}
_CounterVal g_counter;

//------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        StackWP<T>::~StackWP()
        {
            while (m_pHead)
            {
                StackWPNode_p<T> pNext{ m_pHead->Next() };
                delete m_pHead;
                m_pHead = pNext;
            }
            m_size = 0;
        }
    }  // end namespace CppScratch
}  // end namespace NaM

//------------------------------------------------------------------------
class TestRunCerr
{
private:
    static std::uint64_t ms_runId;
    std::uint64_t m_runId;
    std::string m_name;
public:
    TestRunCerr() = delete;
    TestRunCerr(const TestRunCerr&) = delete;
    TestRunCerr(TestRunCerr&&) = delete;

    TestRunCerr(const std::string& name) : m_runId(++ms_runId), m_name(name)
    {
        std::cerr << std::endl << DASHES << std::endl;
        std::cerr << ">>> Start run " << RunId() << ": " << Name() << " >>>" << std::endl;
        std::cerr << DASHES << std::endl;
    }
    ~TestRunCerr()
    {
        std::cerr << "<<< End run " << RunId() << ": " << Name() << " <<<" << std::endl;
        std::cerr << DASHES << std::endl;
        m_name.clear();
        m_runId = std::uint64_t(0);
    }

    [[nodiscard]] inline const std::uint64_t RunId() const { return m_runId; }
    [[nodiscard]] inline const std::string& Name() const { return m_name; }

    TestRunCerr& operator=(const TestRunCerr&) = delete;
    TestRunCerr& operator=(TestRunCerr&) = delete;
};
std::uint64_t TestRunCerr::ms_runId = 0;

struct DummyData
{
    std::string name;
    std::int32_t val;
    bool is;

    DummyData() : name(""), val(INT32_MAX), is{ false }
    {
        std::cerr << g_counter << ") DummyData::DummyData()" << std::endl;
    }
    DummyData(const std::string& _name, const std::int32_t _val, const bool& _is)
        : name(_name), val(_val), is{ _is }
    {
        std::cerr << g_counter << ") DummyData::DummyData(name, val, is)" << std::endl;
    }
    DummyData(const DummyData& orig) : name(orig.name), val(orig.val), is(orig.is)
    {
        std::cerr << g_counter << ") DummyData::DummyData(orig)" << std::endl;
    }
    DummyData(DummyData& other) : name(other.name), val(other.val), is(other.is)
    {
        std::cerr << g_counter << ") DummyData::DummyData(other)" << std::endl;
    }
    virtual ~DummyData()
    {
        std::cerr << g_counter << ") DummyData::~DummyData()" << std::endl;
        name.clear();
    }
    DummyData& operator=(const DummyData& rhs)
    {
        std::cerr << g_counter << ") DummyData::operator=(assign)" << std::endl;
        name.assign(rhs.name);
        val = rhs.val;
        is = rhs.is;
        return *this;
    }
    DummyData& operator=(DummyData&& rhs) noexcept
    {
        std::cerr << g_counter << ") DummyData::operator=(move)" << std::endl;
        name = rhs.name;
        val = rhs.val;
        is = rhs.is;
        return *this;
    }
    const std::string ToString() const
    {
        std::stringstream ss;
        ss << "[ Name: \"" << name << "\", Value: " << val << ", Is: " << BoolStr(is) << " ]";
        return ss.str();
    }
};

std::ostream& operator<<(std::ostream& oss, const DummyData& dummy)
{
    oss << dummy.ToString();
    return oss;
}

using DummyStackWP = NaM::CppScratch::StackWP<DummyData>;
using DummyStackWP_p = NaM::CppScratch::StackWP<DummyData>*;
using DummyStackWPNode_p = NaM::CppScratch::StackWPNode<DummyData>*;
using TEST_NodeManager = NaM::CppScratch::TEST_StackWPNode<DummyData>;

void DummyDataTests()
{
    TestRunCerr nodesRun("DUMMYDATA");

    {
        TestRunCerr run("Default DummyData constructor");
        DummyData d;
        std::cerr << g_counter << ") Default constructor: " << d << std::endl;
    }

    {
        TestRunCerr run("Normal DummyData constructor");

        DummyData d1("test", 1, false);
        std::cerr << g_counter << ") d1() constructor: " << d1 << std::endl;

        DummyData d2{ "test2", 2, true };
        std::cerr << g_counter << ") d2{} constructor: " << d2 << std::endl;
    }

    {
        TestRunCerr run("DummyData copy constructor");
        const DummyData dOrig{"orig", 3, true};
        std::cerr << g_counter << ") Original object: " << dOrig << std::endl;
        DummyData d(dOrig);
        std::cerr << g_counter << ") Copy object: " << d << std::endl;
        std::cerr << g_counter << ") Copied object: " << dOrig << std::endl;
    }

    /*
    // could not trigger a move constructor
    {
        TestRunCerr run("DummyData move constructor");
        DummyData d(DummyData("other", 4, true));
        std::cerr << g_counter << ") Moved object: " << d << std::endl;
    }
    //*/

    {
        TestRunCerr run("DummyData assignment");
        const DummyData dOrig{ "orig", 5, true };
        DummyData d;
        std::cerr << g_counter << ") Original object: " << dOrig << std::endl;
        std::cerr << g_counter << ") Default object: " << d << std::endl;
        d = dOrig;
        std::cerr << g_counter << ") After assignment: " << d << std::endl;
    }

    /*
    // could not trigger a move assignment
    {
        TestRunCerr run("DummyData move assignment");
        DummyData dOrig{ "orig", 6, true };
        DummyData d;
        std::cerr << g_counter << ") Original object: " << dOrig << std::endl;
        std::cerr << g_counter << ") Default object: " << d << std::endl;
        d = dOrig;
        std::cerr << g_counter << ") After assignment: " << d << std::endl;
    }
    //*/
}

void StackWPNodeTests()
{
    TestRunCerr nodesRun("NODES");

    {
        TestRunCerr run("Empty node");
        DummyStackWPNode_p pNode = TEST_NodeManager::NewNode(DummyData("Node data 1", 1, false));
    }
}

void StackWPTests()
{
    TestRunCerr nodesRun("STACK");

    {
        TestRunCerr run("Empty stack");
    }
}
