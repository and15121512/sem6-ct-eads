#include "mm.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>

const size_t BLOCK_SIZE = 7;

const size_t ITERS_CNT = 1000;

struct TestStruct
{
    float k;
    double v;
};

TEST_CASE("ctor")
{
    lab618::CMemoryManager<TestStruct> mm(BLOCK_SIZE, false);
    CHECK(mm.isEmpty());
}

TEST_CASE("newObject and deleteObject (no exceptions, just works)")
{
    lab618::CMemoryManager<TestStruct> mm(BLOCK_SIZE, false);
    std::vector<TestStruct*> ts_arr;

    for (size_t i = 0; i < ITERS_CNT; ++i)
    {
        TestStruct* new_obj = mm.newObject();
        CHECK(nullptr != new_obj);
        CHECK(!mm.isEmpty());
        ts_arr.push_back(new_obj);
    }

    for (size_t i = 0; i < ITERS_CNT; ++i)
    {
        CHECK(mm.deleteObject(ts_arr.back()) == true);
        ts_arr.pop_back();
    }
    CHECK(mm.areAllBlocksEmpty());
}

TEST_CASE("newObject and deleteObject (no exceptions, rand)")
{
    lab618::CMemoryManager<TestStruct> mm(BLOCK_SIZE, true);
    std::vector<TestStruct*> ts_arr;
    ts_arr.push_back(reinterpret_cast<TestStruct*>(&ts_arr));

    for (size_t i = 0; i < ITERS_CNT; ++i)
    {
        bool isPush = rand() % 2;
        if (isPush)
        {
            ts_arr.push_back(mm.newObject());
        }
        else if (!ts_arr.empty())
        {
            mm.deleteObject(ts_arr.back());
            CHECK(mm.deleteObject(ts_arr.back()) == false);
            ts_arr.pop_back();
        }
    }
}

TEST_CASE("newObject, deleteObject, clear (no exceptions, rand)")
{
    lab618::CMemoryManager<TestStruct> mm(BLOCK_SIZE, true);
    std::vector<TestStruct*> ts_arr;
    ts_arr.push_back(reinterpret_cast<TestStruct*>(&ts_arr));

    for (size_t i = 0; i < ITERS_CNT; ++i)
    {
        size_t val = rand() % 9;
        if (val == 0)
        {
            mm.clear();
        }
        else if (1 <= val < 5)
        {
            ts_arr.push_back(mm.newObject());
        }
        else if (!ts_arr.empty())
        {
            mm.deleteObject(ts_arr.back());
            CHECK(mm.deleteObject(ts_arr.back()) == false);
            ts_arr.pop_back();
        }
    }
}

TEST_CASE("newObject, deleteObject, clear (with exceptions, rand)")
{
    lab618::CMemoryManager<TestStruct> mm(BLOCK_SIZE, false);
    std::vector<TestStruct*> ts_arr;
    ts_arr.push_back(reinterpret_cast<TestStruct*>(&ts_arr));

    for (size_t i = 0; i < ITERS_CNT; ++i)
    {
        size_t val = rand() % 9;
        if (val == 0)
        {
            try {
                mm.clear();
            }
            catch (lab618::CMemoryManager<TestStruct>::CException) {
                
            }
        }
        else if (1 <= val < 5)
        {
            ts_arr.push_back(mm.newObject());
        }
        else if (!ts_arr.empty())
        {
            mm.deleteObject(ts_arr.back());
            CHECK(mm.deleteObject(ts_arr.back()) == false);
            ts_arr.pop_back();
        }
    }

    while (!ts_arr.empty())
    {
        mm.deleteObject(ts_arr.back());
        ts_arr.pop_back();
    }
}
