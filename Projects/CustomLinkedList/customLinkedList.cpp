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

        template<typename T> using SingleLinkedListNode_p = SingleLinkedListNode<T>*;

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

            virtual ~SingleLinkedListNode() {}

        public:
            [[nodiscard]] inline const std::uint64_t Id() const { return m_id; }
            [[nodiscard]] inline const T& Data() const { return m_data; }
            [[nodiscard]] inline T& Data() { return m_data; }
            [[nodiscard]] const T& operator*() const { return Data(); }
            [[nodiscard]] T& operator*() { return Data(); }
            SingleLinkedListNode& Data(const T& newData);

            [[nodiscard]] inline const SingleLinkedListNode_p<T> Next() const { return m_pNext; }
            [[nodiscard]] inline SingleLinkedListNode_p<T> Next() { return m_pNext; }

            //
        };

        template<typename T>
        std::uint64_t SingleLinkedListNode<T>::ms_id{0};

        template<typename T>
        class SingleLinkedList
        {
        private:
            static std::uint64_t ms_id;

            SingleLinkedListNode_p<T> m_pHead;
            size_t m_length;

        protected:

        public:
            SingleLinkedList() : m_pHead(nullptr), m_length(0) { ++ms_id; }
            virtual ~SingleLinkedList();
        };

        template<typename T>
        std::uint64_t SingleLinkedList<T>::ms_id{0};

    }; // end namespace CppScratch
}; // end namespace NaM

//------------------------------------------------------------------------
struct Point2D
{
    std::uint32_t x{0}, y{ 0 };
};

using P2D_Node = NaM::CppScratch::SingleLinkedListNode<Point2D>;
using P2D_Node_p = NaM::CppScratch::SingleLinkedListNode_p<Point2D>;
using TEST_P2D_Node = NaM::CppScratch::TEST_SingleLinkedListNode<Point2D>;

std::ostream& operator<<(std::ostream& oss, const Point2D& pt)
{
    oss << "[" << pt.x << ", " << pt.y << "]";
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const P2D_Node_p pNode)
{
    static const std::string nullval = std::string(sizeof(P2D_Node_p), '0');
    if (pNode)
        oss << std::hex << std::setfill('0') << std::setw(sizeof(pNode)) << (void*)pNode;
    else
        oss << "0x" << nullval;
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const P2D_Node& node)
{
    const Point2D& pt(node.Data());
    oss << "Id: " << node.Id() << ", "
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

int main(int argc, char* argv[])
{
    std::cout << "Version: " << CustomLinkedList_VersionFull << std::endl << std::endl;

    P2D_Node_p pNode = TEST_P2D_Node::NewNode(Point2D{0,0});
    ErrPrintP2DNode(pNode);

    TEST_P2D_Node::DeleteNode(pNode);
    ErrPrintP2DNode(pNode);

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        SingleLinkedListNode<T>& SingleLinkedListNode<T>::Data(const T& newData)
        {
            m_data = newData;
            return *this;
        }

        template<typename T>
        SingleLinkedList<T>::~SingleLinkedList()
        {
            std::cerr << "TODO: implement destructor" << std::endl;
        }

    };  // end namespace CppScratch
};  // end namespace NaM
