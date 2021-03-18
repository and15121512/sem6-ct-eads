#include "sort.h"

#include <string>

namespace templates
{
    void merge(void** ppArray, size_t begin, size_t end, CompareSortType pCompareFunc)
    {
        size_t mid = (begin + end) / 2;
        _ASSERT((begin < mid) && (mid < end));

        void** merged = new void*[end - begin];

        size_t i = begin;
        size_t j = mid;
        for (; i < mid && j < end;) {
            for (; i < mid && pCompareFunc(ppArray[i], ppArray[j]) >= 0; ++i)
            {
                merged[(i - begin) + (j - mid)] = ppArray[i];
            }
            for (; i < mid && j < end && pCompareFunc(ppArray[i], ppArray[j]) < 0; ++j)
            {
                merged[(i - begin) + (j - mid)] = ppArray[j];
            }
        }
        for (; i < mid; ++i)
        {
            merged[(i - begin) + (j - mid)] = ppArray[i];
        }
        for (; j < end; ++j)
        {
            merged[(i - begin) + (j - mid)] = ppArray[j];
        }

        for (size_t i = 0; i < end - begin; ++i)
        {
            ppArray[i + begin] = merged[i];
        }

        delete[] merged;
    }

    void mergeSortRec(void** ppArray, size_t begin, size_t end, CompareSortType pCompareFunc) {
        _ASSERT(end - begin >= 1);
        if (end - begin == 1) {
            return;
        }

        size_t mid = (begin + end) / 2;
        mergeSortRec(ppArray, begin, mid, pCompareFunc);
        mergeSortRec(ppArray, mid, end, pCompareFunc);
        merge(ppArray, begin, end, pCompareFunc);
    }

    void mergeSort(void** ppArray, int length, CompareSortType pCompareFunc) {
        if ((nullptr == ppArray) || (length <= 0) || (nullptr == pCompareFunc))
        {
            return;
        }
        mergeSortRec(ppArray, 0, length, pCompareFunc);
    }
}
