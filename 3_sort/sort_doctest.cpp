#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sort.h"

#include <array>
#include <limits>
#include <strstream>
#include <utility>
#include <algorithm>
#include <vector>

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

int cmpTestStruct(const TestStruct* ps1, const TestStruct* ps2) {
    return ps2->key.compare(ps1->key);
}

const char ALPHABET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÛÚÜİŞßôáâãäå¸æçèéêëìíîïğñòóôõö÷øùûúüışÿ";
const int ALPHABET_SIZE = sizeof(ALPHABET);

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    
    for (size_t i = 0; i < length; ++i)
    {
        s += ALPHABET[rand() % ALPHABET_SIZE];
    }

    s += "tmp";
    return s;
}

static void generate(TestStruct *pts)
{
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

const size_t ELEMENTS_COUNTS[] = { 0, 1, 2, 10000 };

TEST_CASE("just works")
{
    for (size_t cnt_i = 0; cnt_i < 4; ++cnt_i) {
        const size_t ELEMENTS_COUNT = ELEMENTS_COUNTS[cnt_i];
        TestStruct* ts_arr = new TestStruct[ELEMENTS_COUNT];
        TestStruct** ts_ptrs_arr = new TestStruct*[ELEMENTS_COUNT];
        std::vector<std::string> keys_vec(ELEMENTS_COUNT);
        for (size_t i = 0; i < ELEMENTS_COUNT; ++i)
        {
            generate(&(ts_arr[i]));
            ts_ptrs_arr[i] = &(ts_arr[i]);
            keys_vec[i] = ts_arr[i].key;
        }

        templates::mergeSort(ts_ptrs_arr, ELEMENTS_COUNT, cmpTestStruct);
        std::sort(keys_vec.begin(), keys_vec.end());

        for (size_t i = 0; i < ELEMENTS_COUNT; ++i)
        {
            CHECK(keys_vec[i] == ts_ptrs_arr[i]->key);
        }

        delete[] ts_arr;
        delete[] ts_ptrs_arr;
    }
}
