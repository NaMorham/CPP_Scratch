#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "customStackVer.h"

#include "NaM/identifiable.h"
#include "NaM/testUtils.h"
#include "NaM/TestObjects/testObjects.h"

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

        // TODO: Modify to use Identifiable base class
        template<typename T>
        class StackWPNode
        {
        private:
            static std::uint64_t ms_ids;
            std::uint64_t m_nodeId;
            T m_data;
            StackWPNode_p<T> m_pNext;

            friend class NaM::CppScratch::StackWP<T>;
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

            [[nodiscard]] std::string ToString() const;
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

            [[nodiscard]] inline const size_t Size() const { return m_size; }

            [[nodiscard]] std::string ToString() const;

            StackWP& Push(const T& newData);
            const T Pop();

            [[nodiscard]] inline const T Peek() const { return (m_pHead ? m_pHead->Data() : T{}); }
            // Not sure we should allow a non const Peek
            // [[nodiscard]] T& Peek() { return (m_pHead ? m_pHead->Data() : T{}); }
        };

        template<typename T>
        std::uint64_t StackWPNode<T>::ms_ids{0};

        template<typename T>
        std::uint64_t StackWP<T>::ms_ids{0};

    }  // end namespace CppScratch
}  // end namespace NaM

//------------------------------------------------------------------------
#if 0
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
const std::string EQUALS(std::string(70, '='));
const std::string nullvalstr(std::string("0x").append(std::string(sizeof(nullptr_t), '0')));

const std::string& BoolStr(const bool& bVal)
{
    static std::string trueString{"true"};
    static std::string falseString{"false"};
    return (bVal ? trueString : falseString);
}
#endif

//------------------------------------------------------------------------
using NaM::CppScratch::nullvalstr;
using NaM::CppScratch::DASHES;
using NaM::CppScratch::EQUALS;
using NaM::CppScratch::TestRunCerr;

void DummyDataTests();
void StackWPNodeTests();
void StackWPTests();

int main(int argc, char* argv[])
{
    std::cout << NaM::CppScratch::LastToken(argv[0]) << " Version " 
        << CustomStackWithPointers_VersionFull << std::endl;

    DummyDataTests();
    StackWPNodeTests();
    StackWPTests();

    std::cerr << std::endl << std::endl;
    return 0;
}

#if 0
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
    oss << std::dec << std::setfill(' ') << std::setw(3) << counter.Count() << ") ";
    return oss;
}
_CounterVal g_counter;
#endif

//------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        std::string StackWPNode<T>::ToString() const
        {
            std::stringstream ss;
            ss << "Node Id: " << std::setw(2) << Id() << ", Next: " << Next()
                << ", Data: " << Data();
            return ss.str();
        }

        template<typename T>
        StackWP<T>::~StackWP()
        {
            while (m_pHead)
            {
                //std::cerr << g_counter << "delete head node: "
                //    << std::setw(3) << std::setfill(' ') << m_pHead->Id()
                //    << " " << m_pHead << ", Size: " << m_size
                //    << std::endl;
                StackWPNode_p<T> pNext{ m_pHead->Next() };
                delete m_pHead;
                m_pHead = pNext;
                --m_size;
            }
            //std::cerr << g_counter << " after head node:     " << m_pHead
            //    << ", Size: " << m_size
            //    << std::endl;
        }

        template<typename T>
        std::string StackWP<T>::ToString() const
        {
            std::stringstream ss;
            ss << "Stack Id: " << std::setw(2) << Id() << ", Size: "
                << Size() << ", Head: " << Head() << std::endl;
            StackWPNode_p<T> pNode{Head()};
            size_t idx{ 0 };
            while (pNode)
            {
                ++idx;
                ss << "\t[" << std::setw(3) << std::setfill(' ') << idx
                    << "] " << pNode->ToString() << std::endl;
                pNode = pNode->Next();
            }
            return ss.str();
        }

        template<typename T>
        StackWP<T>& StackWP<T>::Push(const T& newData)
        {
            StackWPNode<T>* pNewNode{new StackWPNode(newData)};
            if (pNewNode)
            {
                pNewNode->m_pNext = Head();
                m_pHead = pNewNode;
                ++m_size;
            }
            else
            {
                std::cerr << "Could not allocate node" << std::endl;
            }
            return *this;
        }

        template<typename T>
        const T StackWP<T>::Pop()
        {
            if (m_pHead)
            {
                T data(m_pHead->Data());
                m_pHead = m_pHead->Next();
                --m_size;
                return data;
            }
            else
            {
                return T{};
            }
        }
    }  // end namespace CppScratch
}  // end namespace NaM

//------------------------------------------------------------------------
#if 0
class TestRunCerr
{
private:
    static std::uint64_t ms_runId;
    static size_t ms_depth;
    std::uint64_t m_runId;
    std::string m_name;
    const std::string& m_decorator;

public:
    TestRunCerr() = delete;
    TestRunCerr(const TestRunCerr&) = delete;
    TestRunCerr(TestRunCerr&&) = delete;

    TestRunCerr(const std::string& name)
        : m_runId(++ms_runId), m_name(name), m_decorator(ms_depth == 0 ? EQUALS : DASHES)
    {
        ++ms_depth;
        std::cerr << m_decorator << std::endl;
        std::cerr << ">>> Start run " << RunId() << ": " << Name() << " >>>" << std::endl;
        std::cerr << m_decorator << std::endl;
    }
    ~TestRunCerr()
    {
        std::cerr << m_decorator << std::endl;
        std::cerr << "<<< End run " << RunId() << ": " << Name() << " <<<" << std::endl;
        std::cerr << m_decorator << std::endl;
        --ms_depth;
        if (ms_depth == 0)
            std::cerr << std::endl;
        m_name.clear();
        m_runId = std::uint64_t(0);
    }

    [[nodiscard]] inline const std::uint64_t RunId() const { return m_runId; }
    [[nodiscard]] inline const std::string& Name() const { return m_name; }

    TestRunCerr& operator=(const TestRunCerr&) = delete;
    TestRunCerr& operator=(TestRunCerr&) = delete;
};
std::uint64_t TestRunCerr::ms_runId = 0;
size_t TestRunCerr::ms_depth = 0;
#endif

NaM::CppScratch::_CounterVal g_counter;

#if 0
struct DummyData
{
    std::string name;
    std::int32_t val;
    bool is;

    DummyData()
        : name(""), val(INT32_MAX), is{ false } {}
    DummyData(const std::string& _name, const std::int32_t _val, const bool& _is)
        : name(_name), val(_val), is{ _is } {}
    DummyData(const DummyData& orig)
        : name(orig.name), val(orig.val), is(orig.is) {}
    DummyData(DummyData& other)
        : name(other.name), val(other.val), is(other.is) {}
    virtual ~DummyData() { name.clear(); }

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
        ss << "[ Name: \"" << name << "\", Value: " << val << ", Is: " 
            << NaM::CppScratch::TrueOrFalse(is) << " ]";
        return ss.str();
    }
};

std::ostream& operator<<(std::ostream& oss, const DummyData& dummy)
{
    oss << dummy.ToString();
    return oss;
}
#endif

//------------------------------------------------------------------------
using NaM::CppScratch::TestObjects::DummyData;
using DummyStackWP = NaM::CppScratch::StackWP<DummyData>;
using DummyStackWP_p = NaM::CppScratch::StackWP<DummyData>*;
using DummyStackWPNode_p = NaM::CppScratch::StackWPNode<DummyData>*;
using TEST_NodeManager = NaM::CppScratch::TEST_StackWPNode<DummyData>;

std::ostream& operator<<(std::ostream& oss, DummyStackWPNode_p pNode)
{
    if (pNode)
        oss << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(pNode)) << (void*)pNode << std::dec;
    else
        oss << nullvalstr;
    return oss;
}

std::ostream& operator<<(std::ostream& oss, DummyStackWP_p pNode)
{
    if (pNode)
        oss << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(pNode)) << (void*)pNode << std::dec;
    else
        oss << nullvalstr;
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const DummyStackWP& dummyStack)
{
    oss << dummyStack.ToString();
    return oss;
}

void DummyDataTests()
{
    TestRunCerr nodesRun("DUMMYDATA");

    {
        TestRunCerr run("Default DummyData constructor");
        DummyData d;
        std::cerr << g_counter << "Default constructor: " << d << std::endl;
    }

    {
        TestRunCerr run("Normal DummyData constructor");

        DummyData d1("test", 1, false);
        std::cerr << g_counter << "d1() constructor: " << d1 << std::endl;

        DummyData d2{ "test2", 2, true };
        std::cerr << g_counter << "d2{} constructor: " << d2 << std::endl;
    }

    {
        TestRunCerr run("DummyData copy constructor");
        const DummyData dOrig{"orig", 3, true};
        std::cerr << g_counter << "Original object: " << dOrig << std::endl;
        DummyData d(dOrig);
        std::cerr << g_counter << "Copy object: " << d << std::endl;
        std::cerr << g_counter << "Copied object: " << dOrig << std::endl;
    }

    /*
    // could not trigger a move constructor
    {
        TestRunCerr run("DummyData move constructor");
        DummyData d(DummyData("other", 4, true));
        std::cerr << g_counter << "Moved object: " << d << std::endl;
    }
    //*/

    {
        TestRunCerr run("DummyData assignment");
        const DummyData dOrig{ "orig", 5, true };
        DummyData d;
        std::cerr << g_counter << "Original object: " << dOrig << std::endl;
        std::cerr << g_counter << "Default object: " << d << std::endl;
        d = dOrig;
        std::cerr << g_counter << "After assignment: " << d << std::endl;
    }

    /*
    // could not trigger a move assignment
    {
        TestRunCerr run("DummyData move assignment");
        DummyData dOrig{ "orig", 6, true };
        DummyData d;
        std::cerr << g_counter << "Original object: " << dOrig << std::endl;
        std::cerr << g_counter << "Default object: " << d << std::endl;
        d = dOrig;
        std::cerr << g_counter << "After assignment: " << d << std::endl;
    }
    //*/
}

void StackWPNodeTests()
{
    TestRunCerr nodesRun("NODES");

    {
        TestRunCerr run("Empty node");
        DummyStackWPNode_p pNode = TEST_NodeManager::NewNode(DummyData());
        std::cerr << g_counter << "Default Constructor Node: " << pNode << std::endl;
        std::cerr << g_counter << "\t" << pNode->ToString() << std::endl;
        DummyStackWPNode_p pNode2 = TEST_NodeManager::NewNode(DummyData("Node data 1", 1, false));
        std::cerr << g_counter << "Constructed Node: " << pNode2 << std::endl;
        std::cerr << g_counter << "\t" << pNode2->ToString() << std::endl;

        std::cerr << g_counter << "Free allocated pNodes" << std::endl;
        TEST_NodeManager::DeleteNode(pNode2);
        std::cerr << g_counter << "Freed constructed Node: " << pNode2 << std::endl;
        TEST_NodeManager::DeleteNode(pNode);
        std::cerr << g_counter << "Freed default Node: " << pNode << std::endl;
    }
}

void StackWPTests()
{
    TestRunCerr nodesRun("STACK");

    {
        TestRunCerr run("Empty stack");
        DummyStackWP stk;
        std::cerr << g_counter << "Empty stack: " << std::endl;
        std::cerr << g_counter << "\t" << stk << std::endl;
    }

    {
        TestRunCerr run("Push to stack");
        DummyStackWP stk;
        stk.Push(DummyData{});
        std::cerr << g_counter << "Stack after 1 push: " << std::endl;
        std::cerr << g_counter << "\t" << stk << std::endl;

        size_t numPushes{ 5 };
        bool flipFlop{ false };
        for (size_t idx = 1; idx < numPushes; ++idx)
        {
            std::stringstream ss;
            ss << "DummyData_" << idx;
            stk.Push(DummyData{ ss.str(), std::int32_t(idx) * 2, flipFlop });
            flipFlop = !flipFlop;
        }
        std::cerr << g_counter << "Stack after " << numPushes << " pushes: " << std::endl;
        std::cerr << g_counter << "\t" << stk << std::endl;
    }

    {
        TestRunCerr run("Pop from stack");
        DummyStackWP stk;
        size_t numPushes{ 3 };
        bool flipFlop{ true };
        for (size_t idx = 0; idx < numPushes; ++idx)
        {
            std::stringstream ss;
            ss << "DummyPopData_" << idx;
            stk.Push(DummyData{ ss.str(), std::int32_t(idx) * 2, flipFlop });
            flipFlop = !flipFlop;
        }
        std::cerr << g_counter << "Stack after " << numPushes << " pushes: " << std::endl;
        std::cerr << g_counter << "\t" << stk << std::endl;

        while (stk.Size())
        {
            std::cerr << g_counter << "Before pop) Size: " << stk.Size()
                << ", Head: " << stk.Head() << std::endl;
            DummyData d{ stk.Pop() };
            std::cerr << g_counter << "After pop) Size: " << stk.Size()
                << ", Head: " << stk.Head() << std::endl;
            std::cerr << g_counter << "\tPopped data: " << d << std::endl
                << std::endl;
        }
    }

    {
        TestRunCerr run("Peek top of stack");
        DummyStackWP stk;
        stk.Push(DummyData{ "peek1", 42, true});
        stk.Push(DummyData{ "peek2", 7, true });
        std::cerr << g_counter << "Stack after 2 pushes: " << std::endl;
        std::cerr << g_counter << "\t" << stk << std::endl;

        DummyData d(stk.Peek());
        std::cerr << g_counter << "\tPeeked data: " << d << std::endl
            << std::endl;
    }
}
