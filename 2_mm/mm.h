#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

#include <iostream>

namespace lab618
{
    template <class T>
    class CMemoryManager
    {
    private:
        struct block
        {
            // Массив данных блока
            T* pdata;
            // Адрес следующего блока
            block *pnext;
            // Первая свободная ячейка
            int firstFreeIndex;
            // Число заполненных ячеек
            int usedCount;

            int getNext(int i) const
            {
                _ASSERT(i != -1);
                return *(reinterpret_cast<int*>(&(pdata[i])));
            }

            void setNext(int i, int next) {
                memset(reinterpret_cast<void*>(&(pdata[i])), 0, sizeof(T));
                *(reinterpret_cast<int*>(&(pdata[i]))) = next;
            }
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
        CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false)
            : m_blkSize(_default_block_size),
            m_pBlocks(nullptr),
            m_pCurrentBlk(nullptr),
            m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct)
        {
            _ASSERT(_default_block_size > 0);
        }

        virtual ~CMemoryManager()
        {
            clear();
        }

        // Получить адрес нового элемента из менеджера
        T* newObject()
        {
            if (isEmpty())
            {
                m_pCurrentBlk = newBlock();
            }
            else if (isBlockFull(m_pCurrentBlk))
            {
                block* pcurr = m_pBlocks;
                for (; (nullptr != pcurr) && isBlockFull(pcurr); pcurr = pcurr->pnext)
                {
                }
                m_pCurrentBlk = (nullptr != pcurr) ? pcurr : newBlock();
            }

            T* new_obj_ptr = freeListPop(m_pCurrentBlk);
            constructElement(new_obj_ptr);
            return new_obj_ptr;
        }

        // Освободить элемент в менеджере
        bool deleteObject(T* p)
        {
            block* pcurr_blk = blockWithPtr(p);
            if (nullptr == pcurr_blk)
            {
                return false;
            }
            if (isPtrInFreeList(pcurr_blk, p))
            {
                return false;
            }
            freeListInsert(pcurr_blk, p);
            destructElement(p);
            return true;
        }

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear()
        {
            if (isEmpty())
            {
                return;
            }
            if (!m_isDeleteElementsOnDestruct && !areAllBlocksEmpty())
            {
                throw CException();
            }
            block* pcurr_blk = m_pBlocks;
            for (; nullptr != pcurr_blk;)
            {
                block* pnext = pcurr_blk->pnext;
                deleteBlock(pcurr_blk);
                pcurr_blk = pnext;
            }
            m_pBlocks = nullptr;
            m_pCurrentBlk = nullptr;
        }

        bool isEmpty() const
        {
            return nullptr == m_pBlocks;
        }

        bool areAllBlocksEmpty() const
        {
            _ASSERT(!isEmpty());
            block* pcurr_blk = m_pBlocks;
            for (; (nullptr != pcurr_blk) && isBlockEmpty(pcurr_blk);
                pcurr_blk = pcurr_blk->pnext)
            {
            }
            return nullptr == pcurr_blk;
        }

    private:

        // Создать новый блок данных. применяется в newObject
        block* newBlock() // <--> push to list
        {
            block* pnew_block = new block;

            pnew_block->pdata = reinterpret_cast<T*>(new char[sizeof(T) * m_blkSize]);
            for (size_t i = 0, next = 1; i < m_blkSize - 1; ++i, ++next) {
                pnew_block->setNext(i, next);
            }
            pnew_block->setNext(m_blkSize - 1, -1);
            pnew_block->firstFreeIndex = 0;
            pnew_block->usedCount = 0;

            pnew_block->pnext = m_pBlocks;
            m_pBlocks = pnew_block;
            return pnew_block;
        }

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p)
        {
            _ASSERT(nullptr != p);
            delete[] p->pdata;
            delete p;
        }

        bool isBlockFull(block* pblk) const
        {
            _ASSERT(!isEmpty());
            return -1 == pblk->firstFreeIndex;
        }

        bool isBlockEmpty(block* pblk) const
        {
            _ASSERT(!isEmpty());
            return 0 == pblk->usedCount;
        }

        T* freeListPop(block* pblk) // <--> pop from list of free pos
        {
            _ASSERT(!isEmpty() && !isBlockFull(pblk)); // --> THROW !!!
            ++(pblk->usedCount);
            size_t firstFreeIndexTmp = pblk->firstFreeIndex;
            pblk->firstFreeIndex = pblk->getNext(pblk->firstFreeIndex);
            return pblk->pdata + firstFreeIndexTmp;
        }

        bool isPtrInFreeList(block* pblk, T* p)
        {
            _ASSERT(nullptr != pblk && nullptr != p);
            int curr = pblk->firstFreeIndex;
            for (; curr != -1 && !(curr == p - pblk->pdata); curr = pblk->getNext(curr))
            {
            }
            return curr != -1;
        }

        void freeListInsert(block* pblk, T* p)
        {
            _ASSERT(nullptr != pblk && nullptr != p);
            --(pblk->usedCount);
            if (isBlockFull(pblk) || (p - pblk->pdata) - pblk->firstFreeIndex < 0)
            { // insert before head <--> push
                pblk->setNext(p - pblk->pdata, pblk->firstFreeIndex);
                pblk->firstFreeIndex = p - pblk->pdata;
                return;
            }

            // insert
            int curr = pblk->firstFreeIndex;
            for (; pblk->getNext(curr) != -1 && pblk->getNext(curr) < p - pblk->pdata;
                curr = pblk->getNext(curr))
            {
            }
            pblk->setNext(p - pblk->pdata, pblk->getNext(curr));
            pblk->setNext(curr, p - pblk->pdata);
        }

        block* blockWithPtr(T* p)
        {
            if (isEmpty())
            {
                return nullptr;
            }

            block* pcurr_blk = m_pBlocks;
            for (; (nullptr != pcurr_blk) && !(pcurr_blk->pdata <= p
                && p < pcurr_blk->pdata + m_blkSize);
                pcurr_blk = pcurr_blk->pnext)
            {
            }
            return pcurr_blk;
        }

        void constructElement(T* ptr)
        {
            memset(reinterpret_cast<void*>(ptr), 0, sizeof(T));
            ::new(reinterpret_cast<void*>(ptr)) T;
        }

        void destructElement(T* ptr)
        {
            ptr->T();
            memset(reinterpret_cast<void*>(ptr), 0, sizeof(T));
        }

        // Размер блока
        int m_blkSize; // _ASSERT(m_blkSize > 0)
        // Начало списка блоков
        block* m_pBlocks;
        // Текущий блок
        block *m_pCurrentBlk;
        // Удалять ли элементы при освобождении
        bool m_isDeleteElementsOnDestruct;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2021_02_18
