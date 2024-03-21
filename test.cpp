#include <gtest/gtest.h> // Google Test framework
#include <gmock/gmock.h> // Google Mock framework
#include "queue.h"       // Include the circular queue implementation

using ::testing::_;       // Using wildcard for matching any value
using ::testing::Invoke;  // Using Invoke to specify custom behavior for mocks
using ::testing::NiceMock; // Using NiceMock to create a nice mock with default behavior
using ::testing::Return;   // Using Return to specify return values for mock methods

// Mock class for memory management
class Memory : public IMemory
{
    std::vector<void *> vec; // Vector to track allocated memory pointers

public:
    // Mocking memory allocation method
    MOCK_METHOD(void *, malloc, (size_t size), (override));
    // Mocking memory deallocation method
    MOCK_METHOD(void, free, (void *ptr), (override));

    // Custom memory allocation method to track allocated pointers
    void *allocate(size_t size)
    {
        void *ptr{std::malloc(size)}; // Allocate memory using std::malloc

        if (ptr != nullptr)
        {
            vec.push_back(ptr); // Track allocated pointer in the vector
        }

        return ptr; // Return allocated pointer
    }

    // Custom memory deallocation method to track deallocated pointers
    void release(void *ptr)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end()); // Remove pointer from the vector
        std::free(ptr);                                                  // Deallocate memory using std::free
    }

    // Destructor to ensure all allocated memory is deallocated
    ~Memory()
    {
        EXPECT_EQ(0, vec.size()); // Verify that all allocated memory has been deallocated
    }
};

// Test fixture for the circular queue
template <typename T>
class circleQueueFixture : public ::testing::Test
{
    // Tuple containing vectors of different types of values for testing
    const std::tuple<
        std::vector<int>,
        std::vector<float>,
        std::vector<std::string>>
        allValues{
            {1, 2, 3, 4, 5},                // Integer values
            {1.5f, 2.5f, 3.5f, 4.5f, 5.5f}, // Float values
            {"Pink", "Floyd", "Frank", "Zappa", "DieselTheDog"}}; // String values

protected:
    const std::vector<T> values{std::get<std::vector<T>>(allValues)}; // Extract values of type T from the tuple
    NiceMock<Memory> mock;                                             // NiceMock to create a mock with default behavior
    circleQueue<T> *queue;                                              // Pointer to the circular queue

    // Setup method to initialize the circular queue and write values into it
    void SetUp(void) override
    {
        // Specify behavior for memory allocation mock
        EXPECT_CALL(mock, malloc(_))
            .WillRepeatedly(Invoke(&mock, &Memory::allocate));

        // Specify behavior for memory deallocation mock
        EXPECT_CALL(mock, free(_))
            .WillRepeatedly(Invoke(&mock, &Memory::release));

        // Create a new circular queue with size 5 and mock memory
        queue = new circleQueue<T>(5, mock);
        EXPECT_EQ(0, queue->counter()); // Verify that the queue is initially empty

        // Write values into the queue and verify the counter increments correctly
        for (size_t i = 1; i <= values.size(); i++)
        {
            queue->write(values[i - 1]);
            EXPECT_EQ(i, queue->counter());
        }
    }

    // Teardown method to clean up resources by deleting the circular queue
    void TearDown(void) override
    {
        delete queue; // Delete the circular queue
    }
};

// Define types for which the test suite will be instantiated
using TestTypes = ::testing::Types<int, float, std::string>;

// Instantiate the test suite with type parameterization
TYPED_TEST_SUITE(circleQueueFixture, TestTypes);

TYPED_TEST(circleQueueFixture, MemoryAllocationFailure) {
    EXPECT_CALL(this->mock, malloc(_))
        .WillOnce(Return(nullptr));  

    EXPECT_THROW({
        circleQueue<TypeParam> tempQueue(5, this->mock);
    }, std::bad_alloc);
}


// Test case to verify the read method of the circular queue
TYPED_TEST(circleQueueFixture, readTest)
{
    // Iterate through the values and verify correct reading from the queue
    for (size_t i = 0; i < this->values.size(); i++)
    {
        // Verify the counter decrements correctly
        EXPECT_EQ(this->values.size() - i, this->queue->counter());
        // Verify the correct value is read from the queue
        EXPECT_EQ(this->values[i], this->queue->read());
    }
}


// Test case to verify the write method of the circular queue
TYPED_TEST(circleQueueFixture, writeTest)
{
    // Verify that the queue is initially full
    EXPECT_TRUE(this->queue->isFull());
    // Verify the counter matches the size of the values vector
    EXPECT_EQ(this->values.size(), this->queue->counter());
    // Write a new value to the queue
    this->queue->write(this->values[0]);
    // Verify the counter remains the same after writing to a full queue
    EXPECT_EQ(this->values.size(), this->queue->counter());
    // Verify the correct value is read after writing
    EXPECT_EQ(this->values[1], this->queue->read());
    // Verify the counter decrements after reading
    EXPECT_EQ(this->values.size() - 1, this->queue->counter());
}

// Test case to verify the behavior of the circular queue constructor with invalid sizes
TYPED_TEST(circleQueueFixture, sizeTest)
{
    // Verify that constructing a queue with a size of 2 throws an invalid argument exception
    EXPECT_THROW(circleQueue<TypeParam>(2, this->mock), std::invalid_argument);
    // Verify that constructing a queue with a size of 3 does not throw an exception
    EXPECT_NO_THROW(circleQueue<TypeParam>(3, this->mock));
}

// Test case to verify the move semantics of the circular queue
TYPED_TEST(circleQueueFixture, moveTest)
{
    // Create a temporary queue by moving from the current queue
    circleQueue<TypeParam> temp{std::move(*this->queue)};
    // Verify that the counter of the temporary queue matches the size of the values vector
    EXPECT_EQ(this->values.size(), temp.counter());
    // Verify that the counter of the current queue is 0 after moving
    EXPECT_EQ(0, this->queue->counter());

    // Move the temporary queue back to the current queue
    *this->queue = std::move(temp);
    // Verify that the counter of the current queue matches the size of the values vector
    EXPECT_EQ(this->values.size(), this->queue->counter());
    // Verify that the counter of the temporary queue is 0 after moving
    EXPECT_EQ(0, temp.counter());
}

// Test case to verify the behavior of the empty method of the circular queue
TYPED_TEST(circleQueueFixture, countTest)
{
    // Reset the queue by emptying it
    this->queue->empty();
    // Verify that the counter of the queue is 0 after emptying it
    EXPECT_EQ(this->queue->counter(), 0);
}

// Test case to verify the average method of the circular queue
TYPED_TEST(circleQueueFixture, averageTest)
{
    // Check if the type parameter is arithmetic
    if constexpr (std::is_arithmetic_v<TypeParam>)
    {
        // Write values into the queue
        for (const auto &value : this->values)
        {
            this->queue->write(value);
        }

        // Calculate the sum of values
        TypeParam sum = 0;
        for (const auto &value : this->values)
        {
            sum += value;
        }

        // Calculate the expected average
        double expectTest = static_cast<double>(sum) / this->values.size();

        // Calculate the real average using the queue
        double realTest = this->queue->average();

        // Verify that the real average matches the expected average
        EXPECT_EQ(realTest, expectTest);
    }
}

// Test case to verify the resize method of the circular queue (increase size)
TYPED_TEST(circleQueueFixture, increaseSizeTest)
{
    // Reset the queue by emptying it
    this->queue->empty();

    // Set the new scope (size) of the queue
    size_t newScope = 5;
    // Resize the queue to the new scope
    this->queue->resize(newScope);

    // Verify that the counter of the queue is 0 after resizing
    EXPECT_EQ(this->queue->counter(), 0);

    // Write values into the queue
    for (int i = 0; i < 3; ++i)
    {
        this->queue->write(this->values[i]);
    }

    // Verify that the values are read correctly from the queue after resizing
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(this->queue->read(), this->values[i]);
    }
}

// Test case to verify the resize method of the circular queue (decrease size)
TYPED_TEST(circleQueueFixture, decreaseSizeTest)
{
    // Reset the queue by emptying it
    this->queue->empty();

    // Resize the queue to a smaller scope (size)
    EXPECT_TRUE(this->queue->resize(3));

    // Verify that the counter of the queue is 0 after resizing
    EXPECT_EQ(this->queue->counter(), 0);

    // Write values into the queue
    for (int i = 1; i < 5; ++i)
    {
        this->queue->write(this->values[i]);
    }

    // Verify that the queue only contains the last 3 values after resizing
    EXPECT_EQ(this->queue->counter(), 3);
    for (int i = 2; i < 5; ++i)
    {
        EXPECT_EQ(this->queue->read(), this->values[i]);
    }
}


// Test case to verify the resize method of the circular queue (decrease size to current size)
TYPED_TEST(circleQueueFixture, currentSizeTest)
{
    // Resize the queue to the same size (no change)
    EXPECT_TRUE(this->queue->resize(3));

    // Verify that the counter of the queue matches the resized scope
    EXPECT_EQ(this->queue->counter(), 3);

    // Write a value into the queue
    this->queue->write(this->values[1]);

    // Verify that the counter remains the same after writing to a full queue
    EXPECT_EQ(this->queue->counter(), 3);

    // Verify that the correct values are read from the queue after resizing
    EXPECT_EQ(this->queue->read(), this->values[3]);
    EXPECT_EQ(this->queue->read(), this->values[4]);
    EXPECT_EQ(this->queue->read(), this->values[1]);
}
