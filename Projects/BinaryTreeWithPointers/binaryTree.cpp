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
            TEST_BinaryTreeWPNode(const TEST_BinaryTreeWPNode<T, FnLE>&) = delete;
            TEST_BinaryTreeWPNode(TEST_BinaryTreeWPNode<T, FnLE>&) = delete;
            ~TEST_BinaryTreeWPNode() = default;

            BinaryTreeWPNode<T, FnLE>* CreateNode(const T& data)
            {
                return new BinaryTreeWPNode<T, FnLE>(data);
            }
            void DeleteNode(BinaryTreeWPNode<T, FnLE>*& pNode)
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
                : Identifiable<BinaryTreeWPNode<T, FnLE>>("BinaryTreeWPNode"), m_data(data), m_depth{ 0 },
                m_pParent{ nullptr }, m_pLeft{ nullptr }, m_pRight{ nullptr } {}

            BinaryTreeWPNode(const BinaryTreeWPNode<T, FnLE>&) = delete;
            BinaryTreeWPNode(BinaryTreeWPNode<T, FnLE>&&) = delete;

            ~BinaryTreeWPNode();

            BinaryTreeWPNode<T, FnLE>* Add(const T& data);
            inline BinaryTreeWPNode<T, FnLE>* operator+=(const T& data) { return Add(data); }

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
                : Identifiable<BinaryTreeWP<T, FnLE>>("BinaryTreeWP"), m_depth{ 0 }, m_pRoot{ nullptr } {}

            [[nodiscard]] inline const size_t Depth() const { return m_depth; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE> Root() const { return m_pRoot;  }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE> Root() { return m_pRoot; }

            BinaryTreeWP<T, FnLE>& Add(const T& data);
            inline BinaryTreeWP<T, FnLE>& operator+=(const T& data) { return Add(data); }
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

std::ostream& operator<<(std::ostream& oss, const Int32BTree& tree)
{
    oss << "Id: " << tree.Id() << ", Depth: " << tree.Depth()
        << ", &RootNode = " << PtrString(tree.Root());
    if (tree.Root())
    {
        oss << std::endl << tree.Root();
    }
    return oss;
}

void TestNode();
void TestBTreeWP();

int main(int argc, char *argv[])
{
    std::cout << FirstToken(LastToken(argv[0], NaM::CppScratch::pathSeparators), ".")
        << " Version " << BinaryTreeWithPointers_VersionFull << std::endl << std::endl;

    TestNode();
    TestBTreeWP();

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

        template<typename T, class FnLE>
        BinaryTreeWPNode<T, FnLE>* BinaryTreeWPNode<T, FnLE>::Add(const T& data)
        {
            if (compare_less_equal(data, Data()))
            {
                // new data is <= our data
                if (m_pLeft)
                {
                    std::cerr << "TODO: Add to left child node" << std::endl;
                    return m_pLeft->Add(data);
                }
                else
                {
                    m_pLeft = new BinaryTreeWPNode<T, FnLE>(data);
                    return m_pLeft;
                }
            }
            else
            {
                if (m_pRight)
                {
                    std::cerr << "TODO: Add to right child node" << std::endl;
                    return m_pRight->Add(data);
                }
                else
                {
                    m_pRight = new BinaryTreeWPNode<T, FnLE>(data);
                    return m_pRight;
                }
            }
            return this;
        }


        template<typename T, class FnLE>
        BinaryTreeWP<T, FnLE>& BinaryTreeWP<T, FnLE>::Add(const T& data)
        {
            if (m_pRoot)
            {
                //we have a root node, add to it
                std::cerr << "TODO: Add to existing node" << std::endl;
                BinaryTreeWPNode<T, FnLE>* root = m_pRoot->Add(data);
            }
            else
            {
                m_pRoot = new BinaryTreeWPNode<T, FnLE>(data);
            }
            return *this;
        }

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
struct LEPred
{
public:
    bool operator()(const std::int32_t a, const std::int32_t b)
    {
        std::cerr << "a = " << a << ", b = " << b << std::endl;
        return a <= b;
    }
};

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

        NaM::CppScratch::TEST_BinaryTreeWPNode<std::int32_t, LEPred> test2;
        NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t, LEPred> pNode3, pNode4;

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
        TestRunCerr suite("Test B-Tree Node (default comparator)");

        class LEPredBad
        {
        public:
            bool operator()(const std::int32_t a, const std::int32_t b)
            {
                std::cerr << "This is a bad predicate (a = " << a << ", b = " << b << ")" << std::endl;
                return a > b;
            }
        };

        NaM::CppScratch::TEST_BinaryTreeWPNode<std::int32_t, LEPredBad> test3;
        NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t, LEPredBad> pNode5, pNode6;

        pNode5 = test3.CreateNode(10);
        std::cerr << g_counter << pNode5 << std::endl;
        pNode6 = test3.CreateNode(9);
        std::cerr << g_counter << pNode6 << std::endl;

        // LEPredBad is ass backwards and does a > b comparison
        bool comp56{ *pNode5 <= *pNode6 }, comp65{ *pNode6 <= *pNode5 };
        std::cerr << "Expect comp56 to be true: comp56 = " << TrueOrFalse(comp56) << std::endl;
        std::cerr << "Expect comp65 to be false: comp65 = " << TrueOrFalse(comp65) << std::endl;
    }
}

void TestBTreeWP()
{
    TestRunCerr suite("Test B-Tree with pointers");

    {
        TestRunCerr run("Construct");

        Int32BTree tree1;
        std::cerr << g_counter << "tree1: " << tree1 << std::endl;
    }

    {
        TestRunCerr run("Add root data - Add(...)");

        Int32BTree tree2;
        std::cerr << g_counter << "tree2: " << tree2 << std::endl;
        tree2.Add(15);
        std::cerr << g_counter << "tree2: " << tree2 << std::endl;
    }

    {
        TestRunCerr run("Add root data - += ...");

        Int32BTree tree3;
        std::cerr << g_counter << "tree3: " << tree3 << std::endl;
        tree3 += 35;
        std::cerr << g_counter << "tree3: " << tree3 << std::endl;
    }

    {
        TestRunCerr run("Add root data +1 node - Add(...)");

        Int32BTree tree4;
        std::cerr << g_counter << "tree4: " << tree4 << std::endl;
        tree4.Add(50);
        std::cerr << g_counter << "tree4: " << tree4 << std::endl;
        tree4.Add(40);
        std::cerr << g_counter << "tree4: " << tree4 << std::endl;
    }
}
