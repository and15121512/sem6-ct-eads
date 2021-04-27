#ifndef AVL_HEAD_H_2021_04_01
#define AVL_HEAD_H_2021_04_01

#include "mm.h"

namespace lab618
{
    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
            T* pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;
        };

    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CAVLTree()
            : m_pRoot(nullptr), m_Memory(1000, true)
        {
        }

        virtual ~CAVLTree()
        {
        }

        bool add(T* pElement)
        {
            if (nullptr == pElement)
            {
                return false;
            }
            bool success = false;
            bool depth_decreased = false;
            m_pRoot = addRec(m_pRoot, pElement, success, depth_decreased);
            return success;
        }

        bool update(T* pElement)
        {
            if (nullptr == pElement)
            {
                return false;
            }
            bool success = false;
            bool depth_decreased = false;
            m_pRoot = updateRec(m_pRoot, pElement, success, depth_decreased);
            return success;
        }

        T* find(const T& pElement)
        {
            leaf* curr = m_pRoot;
            while(nullptr != curr) {
                int compare_res = Compare(&pElement, curr->pData);
                if (compare_res < 0)
                {
                    curr = curr->pRight;
                }
                else if (compare_res > 0)
                {
                    curr = curr->pLeft;
                }
                else
                {
                    return curr->pData;
                }
            }
            return nullptr;
        }

        bool remove(const T& element)
        {
            bool success = false;
            bool depth_decreased = false;
            m_pRoot = removeRec(m_pRoot, element, success, depth_decreased);
            return success;
        }

        void clear()
        {
            m_Memory.clear();
            m_pRoot = nullptr;
        }

    private:
        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;

        leaf* rotateLeft(leaf* node)
        {
            _ASSERT(nullptr != node);
            _ASSERT(nullptr != node->pRight);
            leaf* pivot = node->pRight;

            node->pRight = pivot->pLeft;
            pivot->pLeft = node;
            
            int node_bF = node->balanceFactor;
            int pivot_bF = pivot->balanceFactor;
            if (pivot_bF > 0)
            {
                node->balanceFactor = node_bF - pivot_bF - 1;
                pivot->balanceFactor = (node_bF - pivot_bF - 1 > 0) ? pivot_bF - 1 : node_bF - 2;
            }
            else
            {
                node->balanceFactor = node_bF - 1;
                pivot->balanceFactor = (node_bF - 1 > 0) ? pivot_bF - 1 : pivot_bF + node_bF - 2;
            }

            return pivot;
        }

        leaf* rotateRight(leaf* node)
        {
            _ASSERT(nullptr != node);
            _ASSERT(nullptr != node->pLeft);
            leaf* pivot = node->pLeft;

            node->pLeft = pivot->pRight;
            pivot->pRight = node;

            int node_bF = node->balanceFactor;
            int pivot_bF = pivot->balanceFactor;
            if (pivot_bF < 0)
            {
                node->balanceFactor = node_bF - pivot_bF + 1;
                pivot->balanceFactor = (node_bF - pivot_bF + 1 > 0) ? node_bF + 2 : pivot_bF + 1;
            }
            else
            {
                node->balanceFactor = node_bF + 1;
                pivot->balanceFactor = (node_bF + 1 > 0) ? pivot_bF + node_bF + 2 : pivot_bF + 1;
            }

            return pivot;
        }

        leaf* rotateRightLeft(leaf* node)
        {
            _ASSERT(nullptr != node);
            _ASSERT(nullptr != node->pRight);
            _ASSERT(nullptr != node->pRight->pLeft);
            //_ASSERT(2 == node->balanceFactor);
            //_ASSERT(-1 == node->pRight->balanceFactor);

            node->pRight = rotateRight(node->pRight);
            return rotateLeft(node);
        }

        leaf* rotateLeftRight(leaf* node)
        {
            _ASSERT(nullptr != node);
            _ASSERT(nullptr != node->pLeft);
            _ASSERT(nullptr != node->pLeft->pRight);
            //_ASSERT(-2 == node->balanceFactor);
            //_ASSERT(1 == node->pLeft->balanceFactor);

            node->pLeft = rotateLeft(node->pLeft);
            return rotateRight(node);
        }

        leaf* balance(leaf* node)
        {
            _ASSERT(nullptr != node);
            if (node->balanceFactor < 2 && node->balanceFactor > -2)
            {
                return node;
            }
            if (2 == node->balanceFactor)
            {
                _ASSERT(nullptr != node->pRight);
                return (node->pRight->balanceFactor >= 0) ? rotateLeft(node) : rotateRightLeft(node);
            }
            else if (-2 == node->balanceFactor)
            {
                _ASSERT(nullptr != node->pLeft);
                return (node->pLeft->balanceFactor <= 0) ? rotateRight(node) : rotateLeftRight(node);
            }
            else
            {
                _ASSERT(false);
            }
        }

        leaf* addRec(leaf* node, T* pElement, bool& success, bool& depth_increased)
        {
            _ASSERT(nullptr != pElement);
            if (nullptr == node)
            {
                success = true;
                depth_increased = true;
                leaf* new_node = m_Memory.newObject();
                new_node->pData = pElement;
                new_node->pLeft = nullptr;
                new_node->pRight = nullptr;
                new_node->balanceFactor = 0;
                return new_node;
            }
            bool depth_increased_sub = false;
            int compare_res = Compare(pElement, node->pData);
            if (compare_res < 0)
            {
                node->pRight = addRec(node->pRight, pElement, success, depth_increased_sub);
                if (depth_increased_sub)
                {
                    ++(node->balanceFactor);
                }
            }
            else if (compare_res > 0)
            {
                node->pLeft = addRec(node->pLeft, pElement, success, depth_increased_sub);
                if (depth_increased_sub)
                {
                    --(node->balanceFactor);
                }
            }
            else
            {
                success = false;
                depth_increased = false;
                return node;
            }
            node = balance(node);
            depth_increased = (depth_increased_sub && 0 != node->balanceFactor);
            return node;
        }

        leaf* updateRec(leaf* node, T* pElement, bool& success, bool& depth_increased)
        {
            _ASSERT(nullptr != pElement);
            if (nullptr == node)
            {
                success = false;
                depth_increased = true;
                leaf* new_node = m_Memory.newObject();
                new_node->pData = pElement;
                new_node->pLeft = nullptr;
                new_node->pRight = nullptr;
                new_node->balanceFactor = 0;
                return new_node;
            }
            bool depth_increased_sub = false;
            int compare_res = Compare(pElement, node->pData);
            if (compare_res < 0)
            {
                node->pRight = updateRec(node->pRight, pElement, success, depth_increased_sub);
                if (depth_increased_sub)
                {
                    ++(node->balanceFactor);
                }
            }
            else if (compare_res > 0)
            {
                node->pLeft = updateRec(node->pLeft, pElement, success, depth_increased_sub);
                if (depth_increased_sub)
                {
                    --(node->balanceFactor);
                }
            }
            else
            {
                success = true;
                depth_increased = false;
                return node;
            }
            node = balance(node);
            depth_increased = (depth_increased_sub && 0 != node->balanceFactor);
            return node;
        }

        leaf* moveMinToRootRec(leaf* node, leaf* root, bool& depth_decreased)
        {
            _ASSERT(nullptr != node);
            if (nullptr == node->pLeft)
            {
                depth_decreased = true;
                root->pData = node->pData;
                leaf* right = node->pRight;
                m_Memory.deleteObject(node);
                return right;
            }
            bool depth_decreased_sub = false;
            node->pLeft = moveMinToRootRec(node->pLeft, root, depth_decreased_sub);
            if (depth_decreased_sub)
            {
                ++(node->balanceFactor);
            }
            node = balance(node);
            depth_decreased = (depth_decreased_sub && 0 == node->balanceFactor);
            return node;
        }

        leaf* removeRec(leaf* node, const T& element, bool& success, bool& depth_decreased)
        {
            if (nullptr == node)
            {
                success = false;
                return nullptr;
            }
            bool depth_decreased_sub = false;
            int compare_res = Compare(&element, node->pData);
            if (compare_res < 0)
            {
                node->pRight = removeRec(node->pRight, element, success, depth_decreased_sub);
                if (depth_decreased_sub)
                {
                    --(node->balanceFactor);
                }
            }
            else if (compare_res > 0)
            {
                node->pLeft = removeRec(node->pLeft, element, success, depth_decreased_sub);
                if (depth_decreased_sub)
                {
                    ++(node->balanceFactor);
                }
            }
            else
            {
                success = true;
                if (nullptr == node->pRight)
                {
                    depth_decreased = true;
                    leaf* left = node->pLeft;
                    m_Memory.deleteObject(node);
                    return left;
                }
                node->pRight = moveMinToRootRec(node->pRight, node, depth_decreased_sub);
                if (depth_decreased_sub)
                {
                    --(node->balanceFactor);
                }
            }
            node = balance(node);
            depth_decreased = (depth_decreased_sub && 0 == node->balanceFactor);
            return node;
        }
    };
}; // namespace templates

#endif // #define AVL_HEAD_H_2021_04_01
