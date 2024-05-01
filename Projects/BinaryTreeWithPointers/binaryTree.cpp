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
        template <typename T, class FnLE = std::less_equal<T>, class FnE = std::equal_to<T>>
        class BinaryTreeWP;

        template <typename T, class FnLE = std::less_equal<T>, class FnE = std::equal_to<T>>
        class BinaryTreeWPNode;

        template <typename T, class FnLE = std::less_equal<T>, class FnE = std::equal_to<T>>
        using BinaryTreeWPNode_p = BinaryTreeWPNode<T, FnLE, FnE>*;

        template <typename T, class FnLE = std::less_equal<T>, class FnE = std::equal_to<T>>
        class TEST_BinaryTreeWPNode
        {
        public:
            TEST_BinaryTreeWPNode() = default;
            TEST_BinaryTreeWPNode(const TEST_BinaryTreeWPNode<T, FnLE, FnE>&) = delete;
            TEST_BinaryTreeWPNode(TEST_BinaryTreeWPNode<T, FnLE, FnE>&) = delete;
            ~TEST_BinaryTreeWPNode() = default;

            BinaryTreeWPNode<T, FnLE, FnE>* CreateNode(const T& data)
            {
                return new BinaryTreeWPNode<T, FnLE, FnE>(data);
            }

            void DeleteNode(BinaryTreeWPNode<T, FnLE, FnE>*& pNode)
            {
                delete pNode;
                pNode = nullptr;
            }


            void DeleteTreeNode(BinaryTreeWP<T, FnLE, FnE>& tree,
                    BinaryTreeWPNode_p<T, FnLE, FnE> pNode,
                    BinaryTreeWPNode_p<T, FnLE, FnE>& pLeft,
                    BinaryTreeWPNode_p<T, FnLE, FnE>& pRight)
            {
                tree.Delete(pNode, pLeft, pRight);
            }

            // Prune from the node that matches and pass out the node and it's leaves.  The caller
            // is now resposible for cleaning up the memory with delete.
            void  PruneTreeNode(BinaryTreeWP<T, FnLE, FnE>& tree,
                const T& data, BinaryTreeWPNode_p<T, FnLE, FnE>& pOut)
            {
                tree.Prune(data, pOut);
            }
        };

        template <typename T,
            class FnLE /*= std::less_equal<T>*/, class FnE /*= std::equal_to<T>*/ >
        class BinaryTreeWPNode : public Identifiable<BinaryTreeWPNode<T, FnLE, FnE>>
        {
        private:
            friend class BinaryTreeWP<T, FnLE, FnE>;
            friend class TEST_BinaryTreeWPNode<T, FnLE, FnE>;

            T m_data;
            size_t m_depth;
            BinaryTreeWPNode<T, FnLE, FnE>* m_pParent;
            BinaryTreeWPNode<T, FnLE, FnE>* m_pLeft;
            BinaryTreeWPNode<T, FnLE, FnE>* m_pRight;
            FnLE compare_less_equal;
            FnE compare_equal;

        protected:
            BinaryTreeWPNode(const T& data, const size_t depth = 0)
                : Identifiable<BinaryTreeWPNode<T, FnLE, FnE>>("BinaryTreeWPNode"),
                m_data(data), m_depth{ depth },
                m_pParent{ nullptr }, m_pLeft{ nullptr }, m_pRight{ nullptr } {}

            BinaryTreeWPNode(const BinaryTreeWPNode<T, FnLE, FnE>&) = delete;
            BinaryTreeWPNode(BinaryTreeWPNode<T, FnLE, FnE>&&) = delete;

            BinaryTreeWPNode<T, FnLE, FnE>* Add(const T& data, const size_t depth);
            inline BinaryTreeWPNode<T, FnLE, FnE>* operator+=(const T& data) { return Add(data, m_depth+1); }

        public:
            ~BinaryTreeWPNode();

            [[nodiscard]] inline const T& Data() const { return m_data; }
            [[nodiscard]] inline T& Data() { return m_data; }

            [[nodiscard]] inline const size_t& Depth() const { return m_depth; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE, FnE> Parent() const { return m_pParent; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE, FnE> Parent() { return m_pParent; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE, FnE> Left() const { return m_pLeft; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE, FnE> Left() { return m_pLeft; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE, FnE> Right() const { return m_pRight; }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE, FnE> Right() { return m_pRight; }

            const BinaryTreeWPNode_p<T, FnLE, FnE> Find(const T& data) const;
            BinaryTreeWPNode_p<T, FnLE, FnE> Find(const T& data);

            [[nodiscard]] const bool operator<=(const BinaryTreeWPNode<T, FnLE, FnE>& rhs)
            {
                return compare_less_equal(Data(), rhs.Data());
            }

            [[nodiscard]] const bool operator==(const BinaryTreeWPNode<T, FnLE, FnE>& rhs)
            {
                return compare_equal(Data(), rhs.Data());
            }
            [[nodiscard]] const bool operator==(const T& rhsData)
            {
                return compare_equal(Data(), rhsData);
            }

            std::list<T>& Values(std::list<T>& valueList) const;
        };


        template <typename T,
                  class FnLE /*= std::less_equal<T>*/,
                  class FnE /*= std::equal_to<T>*/>
        class BinaryTreeWP : public Identifiable<BinaryTreeWP<T, FnLE, FnE>>
        {
        private:
            friend class TEST_BinaryTreeWPNode<T, FnLE, FnE>;

            size_t m_depth;
            FnLE compare_less_equal;
            BinaryTreeWPNode<T, FnLE, FnE>* m_pRoot;

        protected:
            BinaryTreeWP<T, FnLE, FnE>&
                Delete(BinaryTreeWPNode_p<T, FnLE, FnE> pNode,
                    BinaryTreeWPNode_p<T, FnLE, FnE>& pLeft,
                    BinaryTreeWPNode_p<T, FnLE, FnE>& pRight);

            // Prune from the node that matches and pass out the node and it's leaves.  The caller
            // is now resposible for cleaning up the memory with delete.
            BinaryTreeWP<T, FnLE, FnE>&
                Prune(const T& data, BinaryTreeWPNode_p<T, FnLE, FnE>& pOut);

        public:
            BinaryTreeWP()
                : Identifiable<BinaryTreeWP<T, FnLE, FnE>>("BinaryTreeWP"),
                  m_depth{ 0 }, m_pRoot{ nullptr } {}
            BinaryTreeWP(const BinaryTreeWP<T, FnLE, FnE>&) = delete;
            BinaryTreeWP(BinaryTreeWP<T, FnLE, FnE>&&) = delete;
            virtual ~BinaryTreeWP();

            [[nodiscard]] inline const size_t Depth() const { return m_depth; }

            [[nodiscard]] inline const BinaryTreeWPNode_p<T, FnLE, FnE> Root() const { return m_pRoot;  }
            [[nodiscard]] inline BinaryTreeWPNode_p<T, FnLE, FnE> Root() { return m_pRoot; }

            BinaryTreeWP<T, FnLE, FnE>& Add(const T& data);
            inline BinaryTreeWP<T, FnLE, FnE>& operator+=(const T& data) { return Add(data); }

            const BinaryTreeWPNode_p<T, FnLE, FnE> Find(const T& data) const;
            BinaryTreeWPNode_p<T, FnLE, FnE> Find(const T& data);

            // Just prune from the node that matches to the leaf nodes
            BinaryTreeWP<T, FnLE, FnE>& Prune(const T& data);

            // Delete a node and the re-insert the leaves one by one.
            BinaryTreeWP<T, FnLE, FnE>& Delete(const T& data);
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

using Int32Node_TEST = NaM::CppScratch::TEST_BinaryTreeWPNode <std::int32_t, std::less_equal<int32_t>, std::equal_to<std::int32_t>> ;
using Int32Node_p = NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t, std::less_equal<int32_t>, std::equal_to<std::int32_t>>;
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
        std::string indent(pNode->Depth(), ' ');
        if (pNode->Left())
        {
            oss << std::endl << indent << "Left: " << pNode->Left();
        }
        if (pNode->Right())
        {
            oss << std::endl << indent << "Right: " << pNode->Right();
        }
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
        template<typename T, class FnLE, class FnE>
        BinaryTreeWPNode<T, FnLE, FnE>::~BinaryTreeWPNode()
        {
            if (m_pLeft)
            {
                delete m_pLeft;
                m_pLeft = nullptr;
            }
            if (m_pRight)
            {
                delete m_pRight;
                m_pRight = nullptr;
            }
        }

        template<typename T, class FnLE, class FnE>
        BinaryTreeWPNode<T, FnLE, FnE>* BinaryTreeWPNode<T, FnLE, FnE>::Add(const T& data, const size_t depth)
        {
            if (compare_less_equal(data, Data()))
            {
                // new data is <= our data
                if (m_pLeft)
                {
                    std::cerr << "TODO: Add to left child node" << std::endl;
                    return m_pLeft->Add(data, m_depth+1);
                }
                else
                {
                    m_pLeft = new BinaryTreeWPNode<T, FnLE, FnE>(data, m_depth+1);
                    if (m_pLeft)
                    {
                        m_pLeft->m_pParent = this;
                    }
                    else
                    {
                        std::cerr << "ERROR: Could not allocate Node" << std::endl;
                    }
                    return m_pLeft;
                }
            }
            else
            {
                if (m_pRight)
                {
                    std::cerr << "TODO: Add to right child node" << std::endl;
                    return m_pRight->Add(data, m_depth+1);
                }
                else
                {
                    m_pRight = new BinaryTreeWPNode<T, FnLE, FnE>(data, m_depth+1);
                    if (m_pRight)
                    {
                        m_pRight->m_pParent = this;
                    }
                    else
                    {
                        std::cerr << "ERROR: Could not allocate Node" << std::endl;
                    }
                    return m_pRight;
                }
            }
            return this;
        }

        template<typename T, class FnLE, class FnE>
        BinaryTreeWPNode_p<T, FnLE, FnE> BinaryTreeWPNode<T, FnLE, FnE>::Find(const T& data)
        {
            if (compare_equal(Data(), data))
            {
                return this;
            }
            else
            {
                BinaryTreeWPNode_p<T, FnLE, FnE> pResult{nullptr};
                if (m_pLeft)
                {
                    pResult = m_pLeft->Find(data);
                }
                if (m_pRight && !pResult)
                {
                    pResult = m_pRight->Find(data);
                }
                return pResult;
            }
        }

        template<typename T, class FnLE, class FnE>
        const BinaryTreeWPNode_p<T, FnLE, FnE> BinaryTreeWPNode<T, FnLE, FnE>::Find(const T& data) const
        {
            if (compare_equal(Data(), data))
            {
                return this;
            }
            else
            {
                BinaryTreeWPNode_p<T, FnLE, FnE> pResult{nullptr};
                if (m_pLeft)
                {
                    pResult = m_pLeft->Find(data);
                }
                if (m_pRight && !pResult)
                {
                    pResult = m_pRight->Find(data);
                }
                return pResult;
            }
        }

        template<typename T, class FnLE, class FnE>
        std::list<T>& BinaryTreeWPNode<T, FnLE, FnE>::Values(std::list<T>& valueList) const
        {
            if (m_pLeft)
            {
                m_pLeft->Values(valueList);
            }
            valueList.push_back(m_data);
            if (m_pRight)
            {
                m_pRight->Values(valueList);
            }
            return valueList;
        }


        template<typename T, class FnLE, class FnE>
        BinaryTreeWP<T, FnLE, FnE>::~BinaryTreeWP()
        {
            m_depth = 0;
            if (m_pRoot)
            {
                delete m_pRoot;
                m_pRoot = nullptr;
            }
        }

        template<typename T, class FnLE, class FnE>
        BinaryTreeWP<T, FnLE, FnE>& BinaryTreeWP<T, FnLE, FnE>::Add(const T& data)
        {
            if (m_pRoot)
            {
                //we have a root node, add to it
                BinaryTreeWPNode<T, FnLE, FnE>* pNewNode = m_pRoot->Add(data, 1);
                if (pNewNode)
                {
                    ++m_depth;
                }
            }
            else
            {
                m_pRoot = new BinaryTreeWPNode<T, FnLE, FnE>(data);
                m_depth = 1;
            }
            return *this;
        }

        template<typename T, class FnLE, class FnE>
        BinaryTreeWPNode_p<T, FnLE, FnE> BinaryTreeWP<T, FnLE, FnE>::Find(const T& data)
        {
            if (m_pRoot)
            {
                return m_pRoot->Find(data);
            }
            else
            {
                return nullptr;
            }
        }

        template<typename T, class FnLE, class FnE>
        const BinaryTreeWPNode_p<T, FnLE, FnE> BinaryTreeWP<T, FnLE, FnE>::Find(const T& data) const
        {
            if (m_pRoot)
            {
                return m_pRoot->Find(data);
            }
            else
            {
                return nullptr;
            }
        }


        template<typename T, class FnLE, class FnE>
        BinaryTreeWP<T, FnLE, FnE>& BinaryTreeWP<T, FnLE, FnE>::Prune(const T& data)
        {
            BinaryTreeWPNode_p<T, FnLE, FnE> pNode{ nullptr };
            Prune(data, pNode);
            delete pNode;

            return *this;
        }

        template<typename T, class FnLE, class FnE>
        BinaryTreeWP<T, FnLE, FnE>& BinaryTreeWP<T, FnLE, FnE>::Prune(const T& data, BinaryTreeWPNode_p<T, FnLE, FnE>& pOut)
        {
            BinaryTreeWPNode_p<T, FnLE, FnE> pNode{ Find(data) };
            if (pNode)
            {
                if (pNode == m_pRoot)
                {
                    // the root is what we want to prune, we are removing it all
                    pNode->m_pParent = nullptr;
                    m_pRoot = nullptr;
                    pOut = pNode;
                }
                else
                {
                    BinaryTreeWPNode_p<T, FnLE, FnE> pParent{ pNode->m_pParent };
                    if (pParent)
                    {
                        if (pParent->m_pLeft == pNode)
                        {
                            // pruning the left node
                            pParent->m_pLeft = nullptr;
                        }
                        if (pParent->m_pRight == pNode)
                        {
                            // pruning the right node
                            pParent->m_pRight = nullptr;
                        }
                        pNode->m_pParent = nullptr;
                        pOut = pNode;
                    }
                    else
                    {
                        // wtf?
                        std::cerr << "Somehow found a node with null parent" << std::endl;
                    }
                }
            }
            return *this;
        }

        template<typename T, class FnLE, class FnE>
        BinaryTreeWP<T, FnLE, FnE>& BinaryTreeWP<T, FnLE, FnE>::Delete(const T& data)
        {
            BinaryTreeWPNode_p<T, FnLE, FnE> pNode;
            this->Prune(data, pNode);
            if (pNode)
            {
                /*
                 *  Start state
                 *
                 *        m_pParent
                 *            |
                 *          pNode
                 *         /     \
                 *        /       \
                 *    pLeft      pRight
                 */
                BinaryTreeWPNode_p<T, FnLE, FnE> pLeft, pRight, pParent;
                pLeft = pNode->Left();
                pRight = pNode->Right();
                pParent = pNode->Parent();
            }
            return *this;
        }


        template<typename T, class FnLE, class FnE>
        BinaryTreeWP<T, FnLE, FnE>&
            BinaryTreeWP<T, FnLE, FnE>::Delete(
                BinaryTreeWPNode_p<T, FnLE, FnE> pNode,
                BinaryTreeWPNode_p<T, FnLE, FnE>& pLeft,
                BinaryTreeWPNode_p<T, FnLE, FnE>& pRight)
        {
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
        std::cerr << "LEPred: a = " << a << ", b = " << b << std::endl;
        return a <= b;
    }
};

struct EQPred
{
public:
    bool operator()(const std::int32_t a, const std::int32_t b)
    {
        std::cerr << "EQPred: a = " << a << ", b = " << b << std::endl;
        return a == b;
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

        NaM::CppScratch::TEST_BinaryTreeWPNode<std::int32_t, LEPred, EQPred> test2;
        NaM::CppScratch::BinaryTreeWPNode_p<std::int32_t, LEPred, EQPred> pNode3, pNode4;

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

    {
        TestRunCerr run("Add multiple nodes - Add(...)");

        Int32BTree tree5;
        std::cerr << g_counter << "tree5: " << tree5 << std::endl;
        std::cerr << std::endl;

        tree5.Add(25);
        std::cerr << g_counter << "tree5: " << tree5 << std::endl;
        std::cerr << std::endl;

        tree5.Add(23).Add(21).Add(19);  // all left nodes
        std::cerr << g_counter << "tree5: " << tree5 << std::endl;
        std::cerr << std::endl;

        tree5.Add(50).Add(22);
        std::cerr << g_counter << "tree5: " << tree5 << std::endl;
        std::cerr << std::endl;
    }

    {
        TestRunCerr run("Find node - Find(...)");

        Int32BTree tree6;
        tree6.Add(100);
        tree6.Add(50).Add(150);
        tree6.Add(25).Add(75).Add(125).Add(175);
        std::cerr << g_counter << "tree6: " << tree6 << std::endl;

        Int32Node_p pFound;
        pFound = tree6.Find(200); // should be nullptr
        std::cerr << g_counter << "  Find(200) = " << PtrString(pFound)
            << " == nullptr ->  " << TrueOrFalse(!pFound) << std::endl;

        pFound = tree6.Find(100); // should be Root()
        std::cerr << g_counter << "  Find(100) = " << PtrString(pFound)
            << " == Root() ->  " << TrueOrFalse(pFound = tree6.Root()) << std::endl;

        pFound = tree6.Find(50); // should be Root().Left()
        std::cerr << g_counter << "  Find(50) = " << PtrString(pFound)
            << " == Root().Left() ->  " << TrueOrFalse(pFound = tree6.Root()->Left())
            << std::endl;

        pFound = tree6.Find(175); // should be Root().Right().Right()
        std::cerr << g_counter << "  Find(175) = " << PtrString(pFound)
            << " == Root().Right().Right() ->  "
            << TrueOrFalse(pFound = tree6.Root()->Right()->Right()) << std::endl;

        pFound = tree6.Find(125); // should be Root().Right().Left()
        std::cerr << g_counter << "  Find(125) = " << PtrString(pFound)
            << " == Root().Right().Left() ->  "
            << TrueOrFalse(pFound = tree6.Root()->Right()->Left()) << std::endl;
    }

    {
        TestRunCerr run("Prune node and children - Prune(...)");

        Int32Node_TEST treeTest7;
        Int32BTree tree7;
        Int32Node_p pRemoved;

        tree7.Add(100);
        tree7.Add(50).Add(150);
        tree7.Add(25).Add(75).Add(125).Add(175);
        std::cerr << g_counter << "tree7: " << tree7 << std::endl;

        std::cerr << std::endl
            << "Use the test wrangler to prune the node and return the result" << std::endl;
        // prune the left tree
        treeTest7.PruneTreeNode(tree7, 50, pRemoved);
        std::cerr << "  Prune(50, out) = " << PtrString(pRemoved) << std::endl;
        std::cerr << g_counter << "tree7: " << tree7 << std::endl << std::endl;
        std::cerr << g_counter << "  out: " << pRemoved << std::endl;

        delete pRemoved;
        pRemoved = nullptr;
    }

    {
        TestRunCerr run("Values of node - Node.Values(...)");

        Int32BTree tree8;
        tree8.Add(100);
        tree8.Add(50).Add(150);
        tree8.Add(25).Add(75).Add(125).Add(175);
        std::cerr << g_counter << "tree8: " << tree8 << std::endl;

        std::list<int32_t> values;
        size_t count{ 0 }, numValues{ tree8.Root()->Values(values).size() };
        std::cerr << g_counter << "Values (" << numValues << ") -> [";

        for (int32_t v : values)
        {
            if (count++)
            {
                std::cerr << ", ";
            }
            std::cerr << v;
        }
        std::cerr << "]" << std::endl;
    }
}
