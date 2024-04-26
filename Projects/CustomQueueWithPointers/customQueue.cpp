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

        template<typename T>
        class QueueWP : public Identifiable<QueueWP<T>>
        {
        private:
            size_t m_size;
            QueueWPNode_p<T> m_pFront;
            QueueWPNode_p<T> m_pBack;

        public:
            QueueWP()
            : Identifiable<QueueWP>("QueueWPNode"),
                m_size{ 0 }, m_pFront{ nullptr }, m_pBack{ nullptr } {}
            QueueWP(const QueueWP& orig);
            QueueWP(QueueWP&& other);
            virtual ~QueueWP();

            [[nodiscard]] inline const size_t Size() const { return m_size; }

            QueueWP& Push(const T& data);

            [[nodiscard]] inline const QueueWPNode_p<T> Front() const { return m_pFront; }
            [[nodiscard]] inline QueueWPNode_p<T> Front() { return m_pFront; }

            [[nodiscard]] inline const QueueWPNode_p<T> Back() const { return m_pBack; }
            [[nodiscard]] inline QueueWPNode_p<T> Back() { return m_pBack; }
        };
    }  // end namespace CppScratch

}  // end namespace NaM

//-----------------------------------------------------------------------------
using NaM::CppScratch::TestObjects::Point2D;
using P2DQueueNode = NaM::CppScratch::QueueWPNode<Point2D>;
using P2DQueueNode_p = NaM::CppScratch::QueueWPNode_p<Point2D>;
using P2DQueue = NaM::CppScratch::QueueWP<Point2D>;
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

std::ostream& operator<<(std::ostream& oss, const P2DQueue& queue)
{
    P2DQueueNode_p pNode{ queue.Front() };

    oss << "Queue (" << queue.Id() << "), Size: " << queue.Size() << ", "
        << "Front (0x" << (void*)queue.Front() << "), Back (0x" << (void*)queue.Back() << ")"
        << std::endl;

    while (pNode)
    {
        oss << g_counter << pNode << std::endl;
        pNode = pNode->Next();
    }

    return oss;
}

void TestP2DNode();
void TestP2DQueue();

int main(int argc, char* argv[])
{
    std::cout << NaM::CppScratch::FirstToken(NaM::CppScratch::LastToken(argv[0]), ".") << std::endl;

    NaM::CppScratch::TestRunCerr run("Main");

    TestP2DNode();
    TestP2DQueue();

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
            /*
            QueueWPNode_p<T> pPrev{ Prev() };
            QueueWPNode_p<T> pNext{ Next() };
            if (m_pPrev)
                m_pPrev->Next(pNext);
            if (m_pNext)
                m_pNext->Prev(pPrev);
            */
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

        //---------------------------------------------------------------------
        template<typename T>
        QueueWP<T>::QueueWP(const QueueWP<T>& orig)
            : Identifiable("QueueWP")
        {
            std::cerr << "TODO: Copy constructor" << std::endl;
        }
        template<typename T>
        QueueWP<T>::QueueWP(QueueWP<T>&& other)
            : Identifiable("QueueWP")
        {
            std::cerr << "TODO: Move constructor" << std::endl;
        }

        template<typename T>
        QueueWP<T>::~QueueWP()
        {
            std::cerr << "TODO: Destructor" << std::endl;
        }

        // push onto the back
        template<typename T>
        QueueWP<T>& QueueWP<T>::Push(const T& data)
        {
            QueueWPNode_p<T> pNewNode = new QueueWPNode<T>(data);
            // the previous node is "ahead" in the queue
            pNewNode->m_pPrev = m_pBack;
            // pNewNode->m_pNext is all ready a nullptr
            if (m_pBack)
                m_pBack->m_pNext = pNewNode; // next is the node after this in the queue
            m_pBack = pNewNode;
            if (!m_pFront)
                m_pFront = pNewNode;
            return *this;
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

void TestP2DQueue()
{
    NaM::CppScratch::TestRunCerr testRun("Queue tests");

    {
        NaM::CppScratch::TestRunCerr testCase("Empty queue");

        P2DQueue q;
        std::cerr << g_counter << q << std::endl;
    }

    {
        NaM::CppScratch::TestRunCerr testCase("Normal queue (push on back)");

        P2DQueue q;
        q.Push(Point2D{3, 4});
        std::cerr << g_counter << q << std::endl;
        q.Push(Point2D{ 5, 6 });
        std::cerr << g_counter << q << std::endl;
    }
}
