#include "mm.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>

const size_t BLOCK_SIZE = 70;

const size_t ITERS_CNT = 10000;

struct TestStruct1 {
    float k;
    double v;
};

class TestStruct2 {
public:
    TestStruct2() = default;
    TestStruct2(const TestStruct2& s);
    TestStruct2& operator=(const TestStruct2& rhs);
    ~TestStruct2() {
        while (!isEmpty()) {
            pop();
        }
    }
    /// Проверяет, является ли стек пустым.
    bool isEmpty() const noexcept;
    /// Добавляет элемент в стек.
    void push(const float value);
    /// Извлекает элемент из стека. Если стек пуст, ничего не делает.
    void pop() noexcept;
    /// Возвращает ссылку на объект на вершине стека. Если стек пуст, бросает исключение std::exception.
    float& top();
    /// Возвращает ссылку на объект на вершине стека. Если стек пуст, бросает исключение std::exception.
    const float& top() const;
private:
    struct Node {
        float value = 0.0f;
        Node* next = nullptr;
    };
private:
    Node* head_ = nullptr;
};

TestStruct2::TestStruct2(const TestStruct2& s) {
    if (nullptr == s.head_) {
        head_ = nullptr;
    }

    Node* curr = nullptr;
    for (Node* curr_s = s.head_; nullptr != curr_s; curr_s = curr_s->next) {
        Node* new_node = new Node;
        new_node->value = curr_s->value;
        new_node->next = nullptr;
        if (nullptr == curr) {
            head_ = new_node;
        }
        else {
            curr->next = new_node;
        }
        curr = new_node;
    }
}

TestStruct2& TestStruct2::operator=(const TestStruct2& rhs) {
    if (this == &rhs) {
        return *this;
    }

    while (!isEmpty()) {
        pop();
    }

    if (nullptr == rhs.head_) {
        head_ = nullptr;
        return *this;
    }

    Node* curr = nullptr;
    for (Node* curr_rhs = rhs.head_; nullptr != curr_rhs; curr_rhs = curr_rhs->next) {
        Node* new_node = new Node;
        new_node->value = curr_rhs->value;
        new_node->next = nullptr;
        if (nullptr == curr) {
            head_ = new_node;
        }
        else {
            curr->next = new_node;
        }
        curr = new_node;
    }
    return *this;
}

void TestStruct2::push(const float value) {
    Node* new_node = new Node;
    new_node->value = value;
    new_node->next = head_;
    head_ = new_node;
}

void TestStruct2::pop() noexcept {
    if (nullptr == head_) {
        return;
    }
    else {
        Node* next = head_->next;
        delete head_;
        head_ = next;
    }
}

float& TestStruct2::top() {
    if (isEmpty()) {
        throw std::exception("top(): StackL is empty");
    }
    return head_->value;
}

const float& TestStruct2::top() const {
    if (isEmpty()) {
        throw std::exception("top(): StackL is empty");
    }
    return head_->value;
}

bool TestStruct2::isEmpty() const noexcept {
    return nullptr == head_;
}

// --- TESTS ------------------------------------------------------------

typedef TestStruct2 TestStruct;

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
