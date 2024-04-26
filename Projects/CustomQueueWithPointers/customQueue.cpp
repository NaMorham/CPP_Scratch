#include <iostream>

#include "customQueueVer.h"

#include "NaM/testUtils.h"
#include "NaM/identifiable.h"
#include "NaM/TestObjects/testPoint2D.h"

namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        class QueueWPNode;

        template<typename T>
        using QueueWPNode_p = QueueWPNode<T>*;

        template<typename T>
        class QueueWP;

        template<typename T>
        using QueueWP_p = QueueWP<T>*;

        template<typename T>
        class TEST_QueueWPNode
        {
        public:
            [[nodiscard]] static QueueWPNode_p<T> NewNode(const T& data)
            {
                return new QueueWPNode<T>(data);
            }
            static void DeleteNode(QueueWPNode_p<T>& nodePtr)
            {
                if (nodePtr)
                {
                    delete nodePtr;
                    nodePtr = nullptr;
                }
            }
        };

        template<typename T>
        class QueueWPNode : public Identifiable<QueueWPNode<T>>
        {
        private:
            QueueWPNode<T>* m_pPrev;
            T m_data;
            QueueWPNode<T>* m_pNext;

            friend class TEST_QueueWPNode<T>;
            friend class QueueWP<T>;

        protected:
            QueueWPNode() = delete;
            QueueWPNode(const T& data)
                : Identifiable<QueueWPNode>("QueueWPNode"),
                  m_pPrev{ nullptr }, m_data{ data }, m_pNext{ nullptr } {}
            QueueWPNode(const QueueWPNode&) = delete;
            QueueWPNode(QueueWPNode&) = delete;
            virtual ~QueueWPNode();

            inline void Prev(QueueWPNode<T>* pPrev) { m_pPrev = pPrev; }
            inline void Next(QueueWPNode<T>* pNext) { m_pNext = pNext; }

        public:
            [[nodiscard]] inline const T& Data() const { return m_data; }
            [[nodiscard]] inline T& Data() { return m_data; }

            void Data(const T& data);

            [[nodiscard]] inline const QueueWPNode<T>* Prev() const { return m_pPrev; }
            [[nodiscard]] inline QueueWPNode<T>* Prev() { return m_pPrev; }

            [[nodiscard]] inline const QueueWPNode<T>* Next() const { return m_pNext; }
            [[nodiscard]] inline QueueWPNode<T>* Next() { return m_pNext; }

            [[nodiscard]] std::string ToString() const;
        };

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
using NaM::CppScratch::TestObjects::Point2D;
using P2DQueueNode = NaM::CppScratch::QueueWPNode<Point2D>;
using P2DQueueNode_p = NaM::CppScratch::QueueWPNode_p<Point2D>;
using TEST_P2DQueueWPNode = NaM::CppScratch::TEST_QueueWPNode<Point2D>;

NaM::CppScratch::_CounterVal g_counter;

std::ostream& operator<<(std::ostream& oss, const P2DQueueNode& node)
{
    oss << node.ToString();
    return oss;
}

std::ostream& operator<<(std::ostream& oss, const P2DQueueNode_p pNode)
{
    oss << "pNode (";
    if (pNode)
    {
        oss << "0x"
            << std::hex << std::setfill('0') << std::setw(sizeof(pNode))
            << (void*)pNode
            << std::dec
            << ") -> [" << pNode->ToString() << "]";
    }
    else
    {
        oss << NaM::CppScratch::nullvalstr << ")";
    }
    return oss;
}

void TestP2DNode();

int main(int argc, char* argv[])
{
    std::cout << NaM::CppScratch::FirstToken(NaM::CppScratch::LastToken(argv[0]), ".") << std::endl;

    NaM::CppScratch::TestRunCerr run("Main");

    TestP2DNode();

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
namespace NaM
{
    namespace CppScratch
    {
        template<typename T>
        QueueWPNode<T>::~QueueWPNode()
        {
            QueueWPNode_p<T> pPrev{ Prev() };
            QueueWPNode_p<T> pNext{ Next() };
            if (m_pPrev)
                m_pPrev->Next(pNext);
            if (m_pNext)
                m_pNext->Prev(pPrev);
        }

        template<typename T>
        void QueueWPNode<T>::Data(const T& data)
        {
            m_data = data;
        }

        template<typename T>
        std::string QueueWPNode<T>::ToString() const {
            std::stringstream ss;
            ss << "Prev (";
            if (Prev())
                ss << Prev();
            else
                ss << NaM::CppScratch::nullvalstr;
            ss << ") <- " << m_data << " -> Next (";
            if (Next())
                ss << Next();
            else
                ss << NaM::CppScratch::nullvalstr;
            ss << ")";
            return ss.str();
        }

    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
void TestP2DNode()
{
    NaM::CppScratch::TestRunCerr run("Node tests");

    P2DQueueNode_p pNode = TEST_P2DQueueWPNode::NewNode(Point2D{ 1, 2 });

    std::cerr << g_counter << "Allocate pNode: " << pNode << std::endl;

    TEST_P2DQueueWPNode::DeleteNode(pNode);

    std::cerr << g_counter << "Delete pNode: " << pNode << std::endl;
}
