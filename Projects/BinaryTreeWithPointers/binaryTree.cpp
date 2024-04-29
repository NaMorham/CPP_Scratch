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
        template <typename T, class FnLE = std::less_equal<T> >
        class BinaryTreeWP;

        template <typename T, class FnLE = std::less_equal<T> >
        class BinaryTreeWPNode;

        template <typename T, class FnLE = std::less_equal<T> >
        using BinaryTreeWPNode_p = BinaryTreeWPNode<T, FnLE>*;

        template <typename T, class FnLE = std::less_equal<T> >
        class TEST_BinaryTreeWPNode
        {
        public:
            TEST_BinaryTreeWPNode() = default;
            TEST_BinaryTreeWPNode(const TEST_BinaryTreeWPNode&) = delete;
            TEST_BinaryTreeWPNode(TEST_BinaryTreeWPNode&) = delete;
            ~TEST_BinaryTreeWPNode() = default;

            BinaryTreeWPNode_p<T, FnLE> CreateNode(const T& data)
            {
                return new BinaryTreeWPNode(data);
            }
            void DeleteNode(BinaryTreeWPNode_p<T, FnLE>& pNode)
            {
                delete pNode;
                pNode = nullptr;
            }
        };

        template <typename T, class FnLE /*= std::less_equal<T>*/ >
        class BinaryTreeWPNode : public Identifiable<BinaryTreeWPNode<T, FnLE>>
        {
        private:
            friend class BinaryTreeWP<T, FnLE>;
            friend class TEST_BinaryTreeWPNode<T, FnLE>;

            T m_data;
            size_t m_depth;
            BinaryTreeWPNode<T, FnLE>* m_pParent;
            BinaryTreeWPNode<T, FnLE>* m_pLeft;
            BinaryTreeWPNode<T, FnLE>* m_pRight;
            FnLE compare_less_equal;

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
            BinaryTreeWPNode<T, FnLE>& SetData(const T& newData);

            [[nodiscard]] inline const size_t& Depth() const { return m_depth; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE> Parent() const { return m_pParent; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE> Parent() { return m_pParent; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE> Left() const { return m_pLeft; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE> Left() { return m_pLeft; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE> Right() const { return m_pRight; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE> Right() { return m_pRight; }

            [[nodiscard]] const bool operator<=(const BinaryTreeWPNode<T, FnLE>& rhs)
            {
                return compare_less_equal(Data(), rhs.Data());
            }
        };

        template <typename T, class FnLE /*= std::less_equal<T>*/ >
        class BinaryTreeWP : public Identifiable<BinaryTreeWP<T, FnLE>>
        {
        protected:
        private:
            size_t m_depth;
            FnLE compare_less_equal;
            BinaryTreeWPNode<T, FnLE>* m_pRoot;

        public:
            BinaryTreeWP()
                : Identifiable<BinaryTreeWP>("BinaryTreeWP"), m_depth{ 0 }, m_pRoot{ nullptr } {}
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

using Int32Node_TEST = NaM::CppScratch::TEST_BinaryTreeWPNode <std::int32_t, std::less_equal<int32_t>> ;
using Int32Node_p = NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t, std::less_equal<int32_t>>;
using Int32BTree = NaM::CppScratch::BinaryTreeWP<std::int32_t>;
using Int32BTree_p = NaM::CppScratch::BinaryTreeWP<std::int32_t>*;

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
        template<typename T, class FnLE>
        BinaryTreeWPNode<T, FnLE>::~BinaryTreeWPNode()
        {
            std::cerr << "TODO: BinaryTreeWPNode desctructor" << std::endl;
        }

        template<typename T, class FnLE>
        BinaryTreeWPNode<T, FnLE>& BinaryTreeWPNode<T, FnLE>::SetData(const T& newData)
        {
            return *this;
        }

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
struct LEPred
{
public:
    bool operator()(std::int32_t a, std::int32_t b)
    {
        std::cerr << "a = " << a << ", b = " << b << std::endl;
        return a <= b;
    }
};
bool LETest(std::int32_t a, int32_t b)
{
    std::cerr << "a = " << a << ", b = " << b << std::endl;
    return a <= b;
}

void TestNode()
{
    {
        TestRunCerr suite("Test B-Tree Node");
        Int32Node_TEST test;

        Int32Node_p pNewNode = test.CreateNode(42);
        std::cerr << g_counter << pNewNode << std::endl; // after create

        test.DeleteNode(pNewNode);
        std::cerr << g_counter << pNewNode << std::endl; // after delete

        pNewNode = test.CreateNode(42);
        Int32Node_p pNode2 = test.CreateNode(12);
        bool comp = ((*pNode2) <= (*pNewNode));
        std::cerr << g_counter << "(*pNode2 <= *pNewNode) = " << TrueOrFalse(comp) << std::endl;
        // cleanup
        test.DeleteNode(pNewNode);
        test.DeleteNode(pNode2);
    }

    {
        TestRunCerr suite("Test B-Tree Node (default comparator)");

        //*
        NaM::CppScratch::TEST_BinaryTreeWPNode<std::int32_t> test2;
        NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t> pNode3, pNode4;
        /*/
        NaM::CppScratch::TEST_BinaryTreeWPNode<std::int32_t, LEPred> test2;
        NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t, LEPred> pNode3, pNode4;
        //*/

        pNode3 = test2.CreateNode(13);
        std::cerr << g_counter << pNode3 << std::endl;
        pNode4 = test2.CreateNode(7);
        std::cerr << g_counter << pNode4 << std::endl;
        std::cerr << g_counter << "(*pNode3 <= *pNode4) = " << TrueOrFalse((*pNode3) <= (*pNode4)) << std::endl;
        std::cerr << g_counter << "(*pNode4 <= *pNode3) = " << TrueOrFalse((*pNode4) <= (*pNode3)) << std::endl;
        test2.DeleteNode(pNode4);
        test2.DeleteNode(pNode3);
    }

    {
    }
}
