#include "queue.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>


TEST(QueueFixture, nothing)
{

}

/*using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

class Memory : public IMemory
{
    std::vector<void *> vec;

public:
    MOCK_METHOD(void *, malloc, (size_t size), (override));
    MOCK_METHOD(void, free, (void *ptr), (override));

    size_t memory_blocks(void) { return vec.size(); }

    void *allocate(size_t size)
    {
        void *ptr{std::malloc(size)};

        if (ptr != nullptr)
        {
            vec.push_back(ptr);
        }

        return ptr;
    }

    void release(void *ptr)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
        std::free(ptr);
    }

    ~Memory() { EXPECT_EQ(0, vec.size()); }
};


template <typename T>
class QueueFixture : public ::testing::Test
{
    const std::tuple<
        std::vector<int>,
        std::vector<float>,
        std::vector<std::string>>
        allValues{
            {1, 2, 3, 4, 5},
            {1.5f, 2.5f, 3.5f, 4.5f, 5.5f},
            {"aaa", "bbb", "ccc", "ddd", "eee"}};

protected:
    const std::vector<T> values{std::get<std::vector<T>>(allValues)};
    NiceMock<Memory> mock;
    Queue<T> queue{mock, 5}; // Initialize queue with maximum size 5

    void SetUp(void) override
    {
        EXPECT_EQ(5, queue.size()); // Initial size of the queue
    }

    void TearDown(void) override {}
};

using TestTypes = ::testing::Types<int, float, std::string>;
TYPED_TEST_SUITE(QueueFixture, TestTypes);

TYPED_TEST(QueueFixture, mallocFails)
{
    EXPECT_CALL(this->mock, malloc(_))
        .WillRepeatedly(Return(nullptr));
    EXPECT_FALSE(this->queue.enqueue(this->values[0]));
}

TYPED_TEST(QueueFixture, testClear)
{
    EXPECT_EQ(this->values.size(), this->queue.size());
    this->queue.clear();
    EXPECT_EQ(0, this->mock.memory_blocks());
    EXPECT_EQ(0, this->queue.size());
}

TYPED_TEST(QueueFixture, testAverage)
{
    if constexpr (std::is_same<TypeParam, int>::value || std::is_same<TypeParam, float>::value)
    {
        double average{0};
        for (size_t i = 0; i < this->values.size(); i++)
        {
            average += static_cast<double>(this->values[i]);
        }
        average /= this->values.size();

        EXPECT_FLOAT_EQ(average, this->queue.average());
    }
}

TYPED_TEST(QueueFixture, testDequeue)
{
    TypeParam item;
    size_t size{this->queue.size()};

    for (size_t i = 1; i <= size; i++)
    {
        EXPECT_TRUE(this->queue.dequeue(item));
        EXPECT_EQ(size - i, this->queue.size());
        EXPECT_EQ(item, this->values[i - 1]);
    }

    EXPECT_FALSE(this->queue.dequeue(item));
}

TYPED_TEST(QueueFixture, testMovable)
{
    Queue<TypeParam> temp{std::move(this->queue)};
    EXPECT_EQ(this->values.size(), temp.size());
    EXPECT_EQ(0, this->queue.size());

    this->queue = std::move(temp);
    EXPECT_EQ(this->values.size(), this->queue.size());
    EXPECT_EQ(0, temp.size());
}
*/
