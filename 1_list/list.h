#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext) : data(_data), pnext(_pnext) 
            { 
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator()
                : m_pCurrent(0), m_pBegin(0)
            {
            }

            CIterator(leaf *p)
                : m_pCurrent(p), m_pBegin(0)
            {
            }

            CIterator(const CIterator &src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return !(m_pBegin == it.m_pBegin && m_pCurrent == it.m_pCurrent);
            }

            void operator++()
            {
                if (0 == m_pBegin && 0 == m_pCurrent) 
                {
                    return;
                }
                else if (0 != m_pBegin && 0 == m_pCurrent)
                {
                    m_pCurrent = m_pBegin;
                    m_pBegin = 0;
                }
                else if (0 == m_pBegin && 0 != m_pCurrent) 
                {
                    m_pCurrent = m_pCurrent->pnext;
                }
                else {
                    _ASSERT(false);
                }
            }

            T& getData()
            {
                _ASSERT(isValid());
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return getData();
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                if (0 != m_pBegin) 
                { // checks case: p != null, pre begin state
                    m_pBegin = 0;
                }
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pBegin = p;
                m_pCurrent = 0;
            }

            bool isValid() 
            {
                // isValid <-> the data can be extracted (by getData())
                return 0 != m_pCurrent;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:

        CSingleLinkedList()
            : m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CSingleLinkedList()
        {
            clear();
        }

        void pushBack(T& data)
        {
            leaf* new_leaf = new leaf(data, 0);
            if (0 == m_pBegin) 
            {
                _ASSERT(0 == m_pEnd);
                m_pBegin = new_leaf;
            }
            else 
            {
                _ASSERT(0 != m_pEnd);
                m_pEnd->pnext = new_leaf;
            }
            m_pEnd = new_leaf;
        }

        void pushFront(T& data)
        {
            leaf* new_leaf = new leaf(data, m_pBegin);
            m_pBegin = new_leaf;
            if (0 == m_pEnd) 
            {
                m_pEnd = new_leaf;
            }
        }

        T popFront()
        {
            _ASSERT(0 != m_pBegin && 0 != m_pEnd);
            T data_tmp = m_pBegin->data;
            leaf* pnext_tmp = m_pBegin->pnext;
            delete m_pBegin;
            if (m_pEnd == m_pBegin) 
            { // size == 1
                m_pEnd = 0;
            }
            m_pBegin = pnext_tmp;
            return data_tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf* pcurr = it.getLeaf();
            _ASSERT(it.isValid());
            _ASSERT(0 != m_pBegin && 0 != m_pEnd);
            if (pcurr == m_pBegin && pcurr == m_pEnd) 
            {
                // "one leaf" case
                delete pcurr;
                it.setLeaf(0);
                m_pBegin = 0;
                m_pEnd = 0;
            }
            else if (pcurr == m_pBegin && pcurr != m_pEnd) 
            {
                // points to the beginning
                m_pBegin = pcurr->pnext;
                it.setLeafPreBegin(m_pBegin);
                delete pcurr;
            }
            else 
            {
                leaf* pprev = m_pBegin;
                for (; pprev->pnext != pcurr; pprev = pprev->pnext)
                {
                }
                it.setLeaf(pprev);
                
                pprev->pnext = pcurr->pnext;
                delete pcurr;

                if (pcurr == m_pEnd) 
                {
                    m_pEnd = pprev;
                }
            }
        }

        int getSize()
        {
            size_t res = 0;
            for (leaf* pcurr = m_pBegin; 0 != pcurr; pcurr = pcurr->pnext) 
            {
                ++res;
            }
            return res;
        }

        void clear()
        {
            while (0 != m_pBegin) 
            { // is not empty
                popFront();
            }
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext)
                : data(_data), pprev(_pprev), pnext(_pnext)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator() 
                : m_pCurrent(0), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(leaf *p)
                : m_pCurrent(p), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(const CIterator &src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
                m_pEnd = src.m_pEnd;
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
                m_pBegin = src.m_pBegin;
                m_pCurrent = src.m_pCurrent;
                m_pEnd = src.m_pEnd;
            }

            bool operator != (const CIterator&  it) const
            {
                return !(m_pBegin == it.m_pBegin 
                    && m_pCurrent == it.m_pCurrent
                    && m_pEnd == it.m_pEnd);
            }

            void operator++()
            {
                if (0 == m_pBegin && 0 == m_pCurrent) 
                {
                    return;
                }
                else if (0 == m_pBegin && 0 != m_pCurrent) 
                {
                    m_pCurrent = m_pCurrent->pnext;
                }
                else if (0 != m_pBegin && 0 == m_pCurrent) 
                {
                    m_pCurrent = m_pBegin;
                    m_pBegin = 0;
                }
                else 
                {
                    _ASSERT(false);
                }
            }

            void operator--()
            {
                if (0 == m_pCurrent && 0 == m_pEnd) 
                {
                    return;
                }
                else if (0 != m_pCurrent && 0 == m_pEnd) 
                {
                    m_pCurrent = m_pCurrent->pprev;
                }
                else if (0 == m_pCurrent && 0 != m_pEnd) 
                {
                    m_pCurrent = m_pEnd;
                    m_pEnd = 0;
                }
                else 
                {
                    _ASSERT(false);
                }
            }

            T& getData()
            {
                _ASSERT(isValid());
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return getData();
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p)
            {
                if (0 != m_pBegin) 
                {
                    m_pBegin = 0;
                }
                if (0 != m_pEnd) 
                {
                    m_pEnd = 0;
                }
                m_pCurrent = p;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                m_pEnd = 0; // in "post end" case
                m_pBegin = p;
                m_pCurrent = 0;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pBegin = 0;
                m_pEnd = p;
                m_pCurrent = 0;
            }

            bool isValid() 
            {
                // isValid <-> the data can be extracted (by getData())
                return 0 != m_pCurrent;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList()
            : m_pBegin(0), m_pEnd(0)
        {
        };

        virtual ~CDualLinkedList()
        {
            clear();
        };

        void pushBack(T& data)
        {
            leaf* new_leaf = new leaf(data, m_pEnd, 0);
            if (0 == m_pBegin && 0 == m_pEnd) 
            { // initially empty
                m_pBegin = new_leaf;
            }
            else if (0 != m_pBegin && 0 != m_pEnd) 
            { // initially not empty
                m_pEnd->pnext = new_leaf;
            }
            else 
            {
                _ASSERT(false);
            }
            m_pEnd = new_leaf;
        }

        T popBack()
        {
            _ASSERT(0 != m_pBegin && 0 != m_pEnd);
            T data_tmp = m_pEnd->data;
            leaf* pprev = m_pEnd->pprev;
            if (m_pBegin == m_pEnd) 
            { // "one leaf" case
                m_pBegin = 0;
            }
            else 
            {
                pprev->pnext = 0;
            }
            delete m_pEnd;
            m_pEnd = pprev;
            return data_tmp;
        }

        void pushFront(T& data)
        {
            leaf* new_leaf = new leaf(data, 0, m_pBegin);
            if (0 == m_pBegin && 0 == m_pEnd) 
            { // initially empty
                m_pEnd = new_leaf;
            }
            else if (0 != m_pBegin && 0 != m_pEnd) 
            { // initially not empty
                m_pBegin->pprev = new_leaf;
            }
            else 
            {
                _ASSERT(false);
            }
            m_pBegin = new_leaf;
        }

        T popFront()
        {
            _ASSERT(0 != m_pBegin && 0 != m_pEnd);
            T data_tmp = m_pBegin->data;
            leaf* pnext = m_pBegin->pnext;
            if (m_pBegin == m_pEnd)
            {
                m_pEnd = 0;
            }
            else 
            {
                pnext->pprev = 0;
            }
            delete m_pBegin;
            m_pBegin = pnext;
            return data_tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf* pcurr = it.getLeaf();
            _ASSERT(it.isValid());
            _ASSERT(0 != m_pBegin && 0 != m_pEnd);
            if (pcurr == m_pBegin && pcurr == m_pEnd) 
            { // "one leaf" case
                delete pcurr;
                it.setLeaf(0);
                m_pBegin = 0;
                m_pEnd = 0;
            }
            else if (pcurr == m_pBegin && pcurr != m_pEnd) 
            { // in the beginning
                leaf* pnext = pcurr->pnext;
                m_pBegin = pnext;
                pnext->pprev = 0;
                it.setLeafPreBegin(m_pBegin);
                delete pcurr;
            }
            else
            {
                leaf* pnext = pcurr->pnext;
                leaf* pprev = pcurr->pprev;
                pprev->pnext = pnext;
                if (pcurr == m_pEnd) 
                {
                    m_pEnd = pprev;
                }
                else 
                {
                    pnext->pprev = pprev;
                }
                it.setLeaf(pprev);
                delete pcurr;
            }
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            leaf* pcurr = it.getLeaf();
            _ASSERT(it.isValid());
            _ASSERT(0 != m_pBegin && 0 != m_pEnd);
            if (pcurr == m_pBegin && pcurr == m_pEnd)
            { // "one leaf" case
                delete pcurr;
                it.setLeaf(0);
                m_pBegin = 0;
                m_pEnd = 0;
            }
            else if (pcurr != m_pBegin && pcurr == m_pEnd)
            { // in the end
                leaf* pprev = pcurr->pprev;
                m_pEnd = pprev;
                pprev->pnext = 0;
                it.setLeafPostEnd(m_pEnd);
                delete pcurr;
            }
            else
            {
                leaf* pnext = pcurr->pnext;
                leaf* pprev = pcurr->pprev;
                pnext->pprev = pprev;
                if (pcurr == m_pBegin)
                {
                    m_pBegin = pnext;
                }
                else
                {
                    pprev->pnext = pnext;
                }
                it.setLeaf(pnext);
                delete pcurr;
            }
        }

        int getSize()
        {
            size_t res = 0;
            for (leaf* pcurr = m_pBegin; 0 != pcurr; pcurr = pcurr->pnext)
            {
                ++res;
            }
            return res;
        }

        void clear()
        {
            while (0 != m_pBegin) 
            {
                popFront();
            }
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

        CIterator end()
        {
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11
