#include <gtest/gtest.h> // Include the Google Test framework
#include <gmock/gmock.h> // Include Google Mock for mocking classes
#include "queue.h" // Include the header file for the circularQueue class

using namespace testing; // Import the testing namespace
using ::testing::Invoke; // Import the Invoke method for mocks
using ::testing::NiceMock; // Import NiceMock for nicer mock declarations
using ::testing::Return; // Import Return for specifying return values in mocks

// Mock memory class for testing
class Memory : public IMemory {
    std::vector<void *> vec;

public:
    // Mock memory functions
    MOCK_METHOD(void *, malloc, (size_t size), (override));
    MOCK_METHOD(void, free, (void *ptr), (override));

    // Function to get the number of memory blocks
    size_t memory_blocks(void) { return vec.size(); }

    // Function to allocate memory
    void *allocate(size_t size)
    {
        void *ptr{std::malloc(size)};

        if (ptr != nullptr)
        {
            vec.push_back(ptr);
        }

        return ptr;
    }

    // Function to release memory
    void release(void *ptr)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
        std::free(ptr);
    }

    // Destructor
    ~Memory() { EXPECT_EQ(0, vec.size()); }
};

template<typename T>
class circularQueueFixture : public ::testing::Test {
    
protected:
    circularQueue<T> queue{5}; // Initialize a circular queue with size 5
};

// Define the types of data for testing
using TestTypes = ::testing::Types<int, float, char>;
TYPED_TEST_SUITE(circularQueueFixture, TestTypes);

// Test case for write and read operations
TYPED_TEST(circularQueueFixture, readAndWriteTest) {
    TypeParam item1 = 42.2;
    TypeParam item2;

    // write an item into the queue
    EXPECT_TRUE(this->queue.write(item1));

    // read the item from the queue
    EXPECT_TRUE(this->queue.read(item2));
    EXPECT_EQ(item2, item1); // Check if the readd item matches the writed item
}

// Test case to check if the queue is full
TYPED_TEST(circularQueueFixture, isFullTest) {
    for (int i = 0; i < 5; ++i) {
        TypeParam item = i;
        EXPECT_TRUE(this->queue.write(item));
    }

    // Check if the queue is full
    EXPECT_TRUE(this->queue.fullQueue());
}


// Test case to check the number of elements in the queue
TYPED_TEST(circularQueueFixture, countElementTest) {
    // Check if the initial count is 0
    EXPECT_EQ(this->queue.count(), 0);

    // write elements into the queue
    for (int i = 0; i < 3; ++i) {
        TypeParam item = i;
        this->queue.write(item);
    }

    // Check if the count matches the number of writed elements
    EXPECT_EQ(this->queue.count(), 3);
}

// Test case to calculate the average of elements in the queue
TYPED_TEST(circularQueueFixture, averageTest) {
    // Initialize a circular queue with data
    circularQueue<TypeParam> queueElement{5};
    for (int i = 1; i <= 8; ++i) {
        queueElement.write(i);
    }

    // Check the calculated average
    EXPECT_EQ(queueElement.average(), 3.0);
}

TYPED_TEST(circularQueueFixture, resizeTest) {
    // Resize the queue to size 4
    this->queue.resize(3); 
    
    // Output a message to the terminal
   
    
    // Check if the count is 0 after resizing
    EXPECT_EQ(this->queue.count(), 0); 

    // write elements into the resized queue
    for (int i = 0; i < 4; ++i) {
        TypeParam item = i;
        this->queue.write(item);
    }

    // Check if the count matches the number of writed elements
    EXPECT_EQ(this->queue.count(), 3);
}