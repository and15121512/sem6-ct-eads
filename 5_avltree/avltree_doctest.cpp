#include "avltree.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>
#include <string>
#include <algorithm>

#include <unordered_map>

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
    const char* kAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const size_t kAlphabetSize = 52;
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    s.resize(length);
    for (size_t i = 0; i < s.size(); ++i)
    {
        s[i] = kAlphabet[rand() % kAlphabetSize];
    }
    s += "tmp";
    return s;
}

static void generate(TestStruct* pts)
{
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

int compareFunc(const TestStruct* pElement1, const TestStruct* pElement2)
{
    return (pElement1->key).compare(pElement2->key);
}

TEST_CASE("construct")
{
    lab618::CAVLTree<TestStruct, compareFunc> ht;
}

TEST_CASE("just works")
{
    const size_t kTestArrSize = 10000;
    lab618::CAVLTree<TestStruct, compareFunc> ht;
    
    TestStruct* ts_arr = new TestStruct[kTestArrSize];
    
    std::vector<size_t> indices(kTestArrSize);
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        indices[i] = i;
    }
    std::random_shuffle(indices.begin(), indices.end());
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        ts_arr[i].key = std::to_string(indices[i]) + "_" + makeRandomString();
        ts_arr[i].value1 = makeRandomString();
        ts_arr[i].value2 = makeRandomString();
    }
    
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(true == ht.add(&(ts_arr[i])));
    }
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(false == ht.add(&(ts_arr[i])));
    }
    
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(nullptr != ht.find(ts_arr[i]));
    }

    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(true == ht.remove(ts_arr[i]));
    }
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(false == ht.remove(ts_arr[i]));
    }

    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(nullptr == ht.find(ts_arr[i]));
    }

    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(false == ht.update(&(ts_arr[i])));
    }
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(true == ht.update(&(ts_arr[i])));
    }

    ht.clear();
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        CHECK(false == ht.remove(ts_arr[i]));
    }

    delete[] ts_arr;
}

TEST_CASE("random")
{
    const size_t kTestArrSize = 10000;

    TestStruct* ts_arr = new TestStruct[kTestArrSize];
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        ts_arr[i].key = rand() % 10;
        ts_arr[i].value1 = makeRandomString();
        ts_arr[i].value2 = makeRandomString();
    }

    lab618::CAVLTree<TestStruct, compareFunc> ht;
    std::unordered_map<std::string, TestStruct> ht_stl;
    for (size_t i = 0; i < kTestArrSize; ++i)
    {
        size_t task = rand() % 5;
        if (0 == task)
        {
            bool is_added = ht.add(&(ts_arr[i]));
            auto it = ht_stl.find(ts_arr[i].key);
            if (ht_stl.end() != it)
            {
                CHECK(!is_added);
            }
            else
            {
                CHECK(is_added);
                ht_stl.insert(std::make_pair(ts_arr[i].key, ts_arr[i]));
            }
        }
        else if (1 == task)
        {
            bool is_removed = ht.remove(ts_arr[i]);;
            auto it = ht_stl.find(ts_arr[i].key);
            if (ht_stl.end() != it)
            {
                CHECK(is_removed);
                ht_stl.erase(it);
            }
            else
            {
                CHECK(!is_removed);
            }
        }
        else if (2 == task)
        {
            bool is_updated = ht.update(&(ts_arr[i]));
            auto it = ht_stl.find(ts_arr[i].key);
            if (ht_stl.end() != it)
            {
                CHECK(is_updated);
                ht_stl[ts_arr[i].key] = ts_arr[i];
            }
            else
            {
                CHECK(!is_updated);
                ht_stl.insert(std::make_pair(ts_arr[i].key, ts_arr[i]));
            }
        }
        else if (3 == task)
        {
            TestStruct* ts = ht.find(ts_arr[i]);
            auto it = ht_stl.find(ts_arr[i].key);
            if (ht_stl.end() != it)
            {
                CHECK(nullptr != ts);
            }
            else
            {
                CHECK(nullptr == ts);
            }
        }
        else
        {
            ht.clear();
            ht_stl.clear();
        }
    }

    delete[] ts_arr;
}
