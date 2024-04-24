#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdint>

#include "customLinkedListVer.h"

namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        class SingleLinkedList;

        template<typename T>
        class SingleLinkedListNode;

        template<typename T>
        using SingleLinkedList_p = SingleLinkedList<T>*;

        template<typename T>
        using SingleLinkedListNode_p = SingleLinkedListNode<T>*;

        template<typename T>
        class TEST_SingleLinkedListNode
        {
        public:
            [[nodiscard]] static SingleLinkedListNode_p<T> NewNode(const T& data)
            { 
                return new SingleLinkedListNode<T>(data);
            }
            static void DeleteNode(SingleLinkedListNode_p<T>& nodePtr)
            {
                if (nodePtr)
                {
                    delete nodePtr;
                    nodePtr = nullptr;
                }
            }
        };

        template<typename T>
        class SingleLinkedListNode
        {
        private:
            friend class SingleLinkedList<T>;
            friend class TEST_SingleLinkedListNode<T>;

            static std::uint64_t ms_id;

            std::uint64_t m_id;
            T m_data;
            SingleLinkedListNode_p<T> m_pNext;

        protected:
            SingleLinkedListNode() = delete;
            SingleLinkedListNode(const T& data) : m_id(++ms_id), m_data(data), m_pNext(nullptr) {}

            SingleLinkedListNode(const SingleLinkedListNode<T>& orig)
                : m_data(orig.Data()), m_pNext(nullptr) {}

            virtual ~SingleLinkedListNode();

        public:
            [[nodiscard]] inline const std::uint64_t Id() const { return m_id; }

            [[nodiscard]] inline const T& Data() const { return m_data; }
            [[nodiscard]] const T& operator*() const { return Data(); }

            [[nodiscard]] inline T& Data() { return m_data; }
            [[nodiscard]] T& operator*() { return Data(); }

            SingleLinkedListNode& Data(const T& newData);

            [[nodiscard]] inline const SingleLinkedListNode_p<T> Next() const { return m_pNext; }
            [[nodiscard]] inline SingleLinkedListNode_p<T> Next() { return m_pNext; }

            [[nodiscard]] const std::string ToString() const;
        };

        template<typename T>
        std::uint64_t SingleLinkedListNode<T>::ms_id{0};

        template<typename T>
        class SingleLinkedList
        {
        private:
            static std::uint64_t ms_id;

            std::uint64_t m_id;
            SingleLinkedListNode_p<T> m_pHead;
            SingleLinkedListNode_p<T> m_pTail;
            size_t m_length;

        protected:

        public:
            /*class Iterator
            {
            private:
                SingleLinkedListNode_p<T> m_pNode;
                SingleLinkedList_p<T> m_pOwner;

            protected:
                Iterator(SingleLinkedList_p<T>) m_pNode(nullptr) {}
            };*/

            SingleLinkedList() : m_id(++ms_id), m_pHead(nullptr), m_pTail(nullptr), m_length(0) {}

            SingleLinkedList(const SingleLinkedList& orig) = delete;
            SingleLinkedList(SingleLinkedList&& other) = delete;

            virtual ~SingleLinkedList();

            SingleLinkedList& operator=(SingleLinkedList& rhs) = delete;
            SingleLinkedList& operator=(SingleLinkedList && other) = delete;

            [[nodiscard]] inline const std::uint64_t Id() const { return m_id; }

            [[nodiscard]] inline const size_t Length() const { return m_length; }

            [[nodiscard]] inline const SingleLinkedListNode_p<T> Head() const { return m_pHead; }
            [[nodiscard]] inline SingleLinkedListNode_p<T> Head() { return m_pHead; }

            [[nodiscard]] inline const SingleLinkedListNode_p<T> Tail() const { return m_pTail; }
            [[nodiscard]] inline SingleLinkedListNode_p<T> Tail() { return m_pTail; }

            SingleLinkedList& Append(const T& newData);
            inline SingleLinkedList& operator+=(const T& newData) { return Append(newData); }

            const std::string ToString() const;
        };

        template<typename T>
        std::uint64_t SingleLinkedList<T>::ms_id{0};

    }; // end namespace CppScratch
}; // end namespace NaM

//------------------------------------------------------------------------
struct Point2D
{
    std::int32_t x{0}, y{ 0 };
};

std::ostream& operator<<(std::ostream& oss, const Point2D& pt)
{
    oss << "[" << pt.x << ", " << pt.y << "]";
    return oss;
}

//------------------------------------------------------------------------
using P2D_Node = NaM::CppScratch::SingleLinkedListNode<Point2D>;
using P2D_Node_p = NaM::CppScratch::SingleLinkedListNode_p<Point2D>;
using TEST_P2D_Node = NaM::CppScratch::TEST_SingleLinkedListNode<Point2D>;

using P2D_List = NaM::CppScratch::SingleLinkedList<Point2D>;
using P2D_List_p = NaM::CppScratch::SingleLinkedList_p<Point2D>;

const std::string DASHES(std::string(70, '-'));

std::ostream& operator<<(std::ostream& oss, const P2D_Node_p pNode)
{
    static const std::string nullval = std::string(sizeof(P2D_Node_p), '0');
    if (pNode)
        oss << std::hex << std::setfill('0') << std::setw(sizeof(pNode)) << (void*)pNode << std::dec;
    else
        oss << "0x" << nullval;
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const P2D_List_p pList)
{
    static const std::string nullval = std::string(sizeof(P2D_List_p), '0');
    if (pList)
        oss << std::hex << std::setfill('0') << std::setw(sizeof(pList)) << (void*)pList << std::dec;
    else
        oss << "0x" << nullval;
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const P2D_Node& node)
{
    const Point2D& pt(node.Data());
    oss << "Id: " << std::setw(2) << node.Id() << ", "
        << "Data: [" << pt.x << ", " << pt.y << "], "
        << "Next: " << node.Next();
    return oss;
}

void ErrPrintP2DNode(const P2D_Node_p pNode)
{
    static const std::string nullval = std::string(sizeof(P2D_Node_p), '0');
    std::cerr << "*pNode (" << pNode << ") = ";
    if (pNode)
        std::cerr << (*pNode);
    else
        std::cerr << "0x" << nullval;
    std::cerr << std::endl;
}

void ErrPrintP2DNode(const P2D_Node_p* ppNode, const size_t num)
{
    static const std::string nullval = std::string(sizeof(P2D_Node_p), '0');
    if (ppNode)
    {
        for (size_t i = 0; i < num; ++i)
        {
            ErrPrintP2DNode(ppNode[i]);
        }
    }
    else
    {
        std::cerr << "0x" << nullval;
    }
    std::cerr << std::endl;
}

std::ostream& operator<<(std::ostream& oss, const P2D_List& theList)
{
    oss << theList.ToString();
    return oss;
}

//------------------------------------------------------------------------
void TestSLLNodes();
void TestSLLLists();

int main(int argc, char* argv[])
{
    std::cout << "Version: " << CustomLinkedList_VersionFull << std::endl << std::endl;

    TestSLLNodes();
    TestSLLLists();

    std::cerr << std::endl << DASHES << std::endl << std::endl;

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        SingleLinkedListNode<T>::~SingleLinkedListNode()
        {
            std::cerr << "Delete Node: " << Id() << std::endl;
        }

        template<typename T>
        SingleLinkedListNode<T>& SingleLinkedListNode<T>::Data(const T& newData)
        {
            m_data = newData;
            return *this;
        }

        template<typename T>
        const std::string SingleLinkedListNode<T>::ToString() const
        {
            std::stringstream ss;
            ss << "Node Id: " << std::setw(2) << Id() << ", Next: " << Next()
               << ", Data: " << Data();
            return ss.str();
        }

        template<typename T>
        SingleLinkedList<T>::~SingleLinkedList()
        {
            std::cerr << "Delete List: " << std::setw(2) << Id() << std::endl;
            SingleLinkedListNode_p<T> pNode{Head()};
            while (pNode)
            {
                SingleLinkedListNode_p<T> pNext{pNode->Next()};
                if (m_pTail = pNode)
                    m_pTail = nullptr;
                delete pNode;
                m_pHead = pNext;
                pNode = pNext;
                --m_length;
                std::cerr << *this << std::endl;
            }
        }

        template<typename T>
        SingleLinkedList<T>& SingleLinkedList<T>::Append(const T& newData)
        {
            SingleLinkedListNode_p<T> pNewNode{ new SingleLinkedListNode<T>{newData} };
            if (pNewNode)
            {
                if (m_pTail)
                {
                    m_pTail->m_pNext = pNewNode;
                    m_pTail = pNewNode;
                }
                else
                {
                    // we're empty
                    m_pHead = m_pTail = pNewNode;
                }
                ++m_length;
            }
            else
            {
                std::cerr << "COULD NOT ALLOCATE NEW NODE" << std::endl;
            }
            return *this;
        }

        template<typename T>
        const std::string SingleLinkedList<T>::ToString() const
        {
            std::stringstream ss;
            ss << "List Id: " << std::setw(2) << Id() << ", Length: " << Length()
               << ", Head: " << Head() << ", Tail: " << Tail();

            size_t idx{ 0 };
            SingleLinkedListNode_p<T> pNode{Head()};
            while (pNode)
            {
                ss << std::endl << "\tNode " << idx++ << ") " << (*pNode);
                pNode = pNode->Next();
            }

            return ss.str();
        }

    };  // end namespace CppScratch
};  // end namespace NaM

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
        std::cerr << "<<< End run " << RunId() << ": " << Name() << " <<<" << std::endl << std::endl;
        m_name.clear();
        m_runId = std::uint64_t(0);
    }

    [[nodiscard]] inline const std::uint64_t RunId() const { return m_runId; }
    [[nodiscard]] inline const std::string& Name() const { return m_name; }

    TestRunCerr& operator=(const TestRunCerr&) = delete;
    TestRunCerr& operator=(TestRunCerr&) = delete;
};
std::uint64_t TestRunCerr::ms_runId = 0;

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
{ oss << std::dec << counter.Count(); return oss; }
_CounterVal g_counter;

void TestSLLNodes()
{
    TestRunCerr nodesRun("NODES");

    {
        TestRunCerr nodeRun("Stand alone nodes");
        P2D_Node_p pNode = TEST_P2D_Node::NewNode(Point2D{ 0,0 });
        ErrPrintP2DNode(pNode);

        std::cerr << "ToString() = \"" << pNode->ToString() << std::endl;

        TEST_P2D_Node::DeleteNode(pNode);
        ErrPrintP2DNode(pNode);

        std::cerr << std::endl << DASHES << std::endl;
        P2D_Node_p pNode2{ TEST_P2D_Node::NewNode(Point2D{3,4}) };
        ErrPrintP2DNode(pNode2);

        std::cerr << "ToString() = \"" << pNode2->ToString() << std::endl;

        TEST_P2D_Node::DeleteNode(pNode2);
        ErrPrintP2DNode(pNode2);
    }

    {
        TestRunCerr nodeArrRun("Array of nodes");
        P2D_Node_p triangle[3] = {
            TEST_P2D_Node::NewNode(Point2D{ 1,1 }),
            TEST_P2D_Node::NewNode(Point2D{ 5,1 }),
            TEST_P2D_Node::NewNode(Point2D{ 5,4 })
        };
        ErrPrintP2DNode(triangle, 3);
        TEST_P2D_Node::DeleteNode(triangle[0]);
        TEST_P2D_Node::DeleteNode(triangle[1]);
        TEST_P2D_Node::DeleteNode(triangle[2]);
        ErrPrintP2DNode(triangle, 3);

        /*
        std::cerr << std::endl;
        P2D_Node_p *triangle2 = new P2D_Node_p[3] {
            TEST_P2D_Node::NewNode(Point2D{ 10, -1 }),
            TEST_P2D_Node::NewNode(Point2D{ 14, -1 }),
            TEST_P2D_Node::NewNode(Point2D{ 14, 2 })
        };
        ErrPrintP2DNode(triangle2, 3);
        TEST_P2D_Node::DeleteNode(triangle2[0]);
        TEST_P2D_Node::DeleteNode(triangle2[1]);
        TEST_P2D_Node::DeleteNode(triangle2[2]);
        ErrPrintP2DNode(triangle2, 3);
        delete[] triangle2;
        triangle2 = nullptr;
        ErrPrintP2DNode(triangle2, 0);
        //*/

//        std::cerr << std::endl;
    }
}

void TestSLLLists()
{
    TestRunCerr listsRun("SINGLE LINKED LISTS");

    {
        TestRunCerr listRun("Empty list");

        P2D_List list1;
        std::cerr << "list1: " << list1 << std::endl;
    }

    {
        TestRunCerr listRun("List append 1");

        P2D_List list2;
        size_t step{ 0 };
        std::cerr << std::setw(2) << ++step << ") list2: " << list2 << std::endl;
        list2.Append(Point2D{ -1, 1 });
        std::cerr << std::setw(2) << ++step << ") list2: " << list2 << std::endl;
    }

    {
        TestRunCerr listRun("List append N");

        P2D_List list3;
        size_t step2{ 0 };
        std::cerr << std::setw(2) << ++step2 << ") list3: " << list3 << std::endl;
        list3.Append(Point2D{ 0, 3 });
        std::cerr << std::setw(2) << ++step2 << ") list3: " << list3 << std::endl;
        list3.Append(Point2D{ 3, 3 });
        std::cerr << std::setw(2) << ++step2 << ") list3: " << list3 << std::endl;
        list3.Append(Point2D{ 0, 6 });
        std::cerr << std::setw(2) << ++step2 << ") list3: " << list3 << std::endl;
        list3.Append(Point2D{ 3, 9 });
        std::cerr << std::setw(2) << ++step2 << ") list3: " << list3 << std::endl;
        list3.Append(Point2D{ 0, 3 });
        std::cerr << std::setw(2) << ++step2 << ") list3: " << list3 << std::endl;
    }

}
