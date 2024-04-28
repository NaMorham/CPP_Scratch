#include <iostream>
#include <sstream>
#include <functional>

#include "binaryTreeVer.h"

#include "NaM/identifiable.h"
#include "NaM/testUtils.h"

#include <list>

//-----------------------------------------------------------------------------
using NaM::CppScratch::FirstToken;
using NaM::CppScratch::LastToken;

namespace NaM
{
    namespace CppScratch
    {
        template <typename T, class Fn = std::less_equal<T> >
        class BinaryTreeWP;

        template <typename T, class Fn = std::less_equal<T> >
        class BinaryTreeWPNode;

        template <typename T, class Fn = std::less_equal<T> >
        using BinaryTreeWPNode_p = BinaryTreeWPNode<T, Fn>*;

        template <typename T, class Fn = std::less_equal<T> >
        class TEST_BinaryTreeWPNode
        {
        public:
            TEST_BinaryTreeWPNode() = default;
            TEST_BinaryTreeWPNode(const TEST_BinaryTreeWPNode&) = delete;
            TEST_BinaryTreeWPNode(TEST_BinaryTreeWPNode&) = delete;
            ~TEST_BinaryTreeWPNode() = default;

            BinaryTreeWPNode_p<T, Fn> CreateNode(const T& data)
            {
                return new BinaryTreeWPNode(data);
            }
            void DeleteNode(BinaryTreeWPNode_p<T, Fn>& pNode)
            {
                delete pNode;
                pNode = nullptr;
            }
        };

        template <typename T, class Fn /*= std::less_equal<T>*/ >
        class BinaryTreeWPNode : public Identifiable<BinaryTreeWPNode<T>>
        {
        private:
            friend class BinaryTreeWP<T, Fn>;
            friend class TEST_BinaryTreeWPNode<T, Fn>;

            T m_data;
            size_t m_depth;
            BinaryTreeWPNode<T, Fn>* m_pParent;
            BinaryTreeWPNode<T, Fn>* m_pLeft;
            BinaryTreeWPNode<T, Fn>* m_pRight;
            Fn m_comp;

        protected:
            BinaryTreeWPNode(const T& data)
                : Identifiable<BinaryTreeWPNode>("BinaryTreeWPNode"), m_data(data), m_depth{ 0 },
                m_pParent{ nullptr }, m_pLeft{ nullptr }, m_pRight{ nullptr } {}

            BinaryTreeWPNode(const BinaryTreeWPNode&) = delete;
            BinaryTreeWPNode(BinaryTreeWPNode&&) = delete;

            ~BinaryTreeWPNode();

        public:
            [[nodiscard]] inline const T& Data() const { return m_data; }
            [[nodiscard]] inline T& Data() { return m_data; }
            BinaryTreeWPNode<T, Fn>& SetData(const T& newData);

            [[nodiscard]] inline const size_t& Depth() const { return m_depth; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, Fn> Parent() const { return m_pParent; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, Fn> Parent() { return m_pParent; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, Fn> Left() const { return m_pLeft; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, Fn> Left() { return m_pLeft; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, Fn> Right() const { return m_pRight; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, Fn> Right() { return m_pRight; }

            [[nodiscard]] const bool operator<=(const BinaryTreeWPNode<T, Fn>& rhs)
            {
                return m_comp(m_data, rhs.Data());
            }
        };

        template <typename T, class Fn /*= std::less_equal<T>*/ >
        class BinaryTreeWP : public Identifiable<BinaryTreeWP<T>>
        {
        protected:
        private:
            size_t m_depth;
            Fn comp_less_equal;
            BinaryTreeWPNode<T>* m_pRoot;

        public:
            BinaryTreeWP()
                : Identifiable("BinaryTreeWP"), m_depth{ 0 }, comp_less_equal(Fn), m_pRoot{ nullptr } {}
        };

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
NaM::CppScratch::_CounterVal g_counter;

using NaM::CppScratch::FirstToken;
using NaM::CppScratch::LastToken;
using NaM::CppScratch::PtrString;
using NaM::CppScratch::TrueOrFalse;

using NaM::CppScratch::TestRunCerr;

using Int32Node_TEST = NaM::CppScratch::TEST_BinaryTreeWPNode <int32_t, std::less_equal<int32_t>> ;
using Int32Node_p = NaM::CppScratch::BinaryTreeWPNode_p<int32_t, std::less_equal<int32_t>>;
using Int32BTree = NaM::CppScratch::BinaryTreeWP<int32_t>;
using Int32BTree_p = NaM::CppScratch::BinaryTreeWP<int32_t>*;

//-----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& oss, const Int32Node_p pNode)
{
    oss << "Node (" << PtrString(pNode) << ")";
    if (pNode)
    {
        oss << " Id: " << pNode->Id() << ", Depth: " << pNode->Depth()
            << ", Parent: " << PtrString(pNode->Parent()) << ", Data: "
            << pNode->Data() << ", Left: " << PtrString(pNode->Left())
            << ", Right: " << PtrString(pNode->Right());
    }
    return oss;
}

void TestNode();

int main(int argc, char *argv[])
{
    std::cout << FirstToken(LastToken(argv[0], NaM::CppScratch::pathSeparators), ".")
        << " Version " << BinaryTreeWithPointers_VersionFull << std::endl << std::endl;

    TestNode();

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        template<typename T, class Fn>
        BinaryTreeWPNode<T, Fn>::~BinaryTreeWPNode()
        {
            std::cerr << "TODO: BinaryTreeWPNode desctructor" << std::endl;
        }

        template<typename T, class Fn>
        BinaryTreeWPNode<T, Fn>& BinaryTreeWPNode<T, Fn>::SetData(const T& newData)
        {
            return *this;
        }

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
void TestNode()
{
    TestRunCerr suite("Test B-Tree Node");
    Int32Node_TEST test;

    Int32Node_p pNewNode = test.CreateNode(42);
    std::cerr << g_counter << pNewNode << std::endl;

    test.DeleteNode(pNewNode);
    std::cerr << g_counter << pNewNode << std::endl;

    pNewNode = test.CreateNode(42);
    Int32Node_p pNode2 = test.CreateNode(12);
    bool comp = ((*pNode2) <= (*pNewNode));
    std::cerr << g_counter << "(*pNode2 <= *pNewNode) = " << TrueOrFalse(comp) << std::endl;
    test.DeleteNode(pNewNode);

    NaM::CppScratch::TEST_BinaryTreeWPNode<int32_t> test2;
    NaM::CppScratch::BinaryTreeWPNode_p<int32_t> pNode3, pNode4;
    pNode3 = test2.CreateNode(13);
    std::cerr << g_counter << pNode3 << std::endl;
    pNode4 = test2.CreateNode(7);
    std::cerr << g_counter << pNode4 << std::endl;
    std::cerr << g_counter << "(*pNode3 <= *pNode4) = " << TrueOrFalse((*pNode3) <= (*pNode4)) << std::endl;
    std::cerr << g_counter << "(*pNode4 <= *pNode3) = " << TrueOrFalse((*pNode4) <= (*pNode3)) << std::endl;
    test.DeleteNode(pNode4);
    test.DeleteNode(pNode3);
}
