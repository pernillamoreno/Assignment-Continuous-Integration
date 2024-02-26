#include "queue.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>


using ::testing::_; // Import Google Mock's underscore symbol
using ::testing::Invoke; // Import Google Mock's Invoke function
using ::testing::NiceMock; // Import Google Mock's NiceMock class
using ::testing::Return; // Import Google Mock's Return function

// stuct based on IMemory
struct Memory : public IMemory
{
    std::vector<void *> vec;

    MOCK_METHOD(void *, malloc, (size_t size), (override));
    MOCK_METHOD(void, free, (void *ptr), (override));

    // WHEN WE ALLOCATE WE PUSH
    // when allocate is called,
    void *allocate(size_t size)
    { // we allocate memeory
        void *ptr{std::malloc(size)};
        // check if the ptr i 0, if not
        if (ptr != nullptr)
        {
            // push( vec.push_back(ptr) to vector
            vec.push_back(ptr);
        }
        return ptr;
    }
    // WHEN WE RELaSE WE "DELETE"
    void release(void *ptr)
    {
        // if remove finds a value it removes it to the end of the vector
        // erace has 3 arguments , begin end and the value we looking for.  Then call release= std::free(ptr);
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
        std::free(ptr);
    }

    // in the distructor we check if the vector is empty or not
    ~Memory() { EXPECT_EQ(0, vec.size());}
};

template <typename T>
class circularQueueFixture: public ::testing::Test
{
  const std::tuple<
        std::vector<int>,
        std::vector<float>,
        std::vector<std::string>>
        allTestValues{
            {1,2,3,4},
            {1.5f, 2.5f, 3.5f, 4.5f},
            {"pink", "floyd","frank","zappa"}};

protected:
   const std::vector<T> values{std::get<std::vector<T>>(allTestValues)};// T Get the sample values for the current type
    NiceMock<Memory> mock; // Create a nice mock object for memory management
    circularQueue<T> queue{mock}; // Create a instance queue object using the mock memory manager

        void SetUp(void) override 
        { // Setup method executed before each test case

             EXPECT_EQ(0,queue.size()); // Assert that the queue is initially empty

              // Set up mock expectations for malloc and free methods
             EXPECT_CALL(mock, malloc(_))
                .WillRepeatedly(Invoke(&mock, &Memory::allocate)); //can be called nuber of times
             EXPECT_CALL(mock, free(_))
                .WillRepeatedly(Invoke(&mock, &Memory::release));

        
            // Enqueue sample values into the queue
            for (size_t i = 1; i <= values.size(); i++)
            {
                EXPECT_TRUE(queue.enqueue(values[i - 1])); // Enqueue each sample value
                EXPECT_EQ(i, queue.size()); // Assert that the size of the queue increases after enqueueing

            }
        }
        void TearDown(void) override {} // Tear down method executed after each test case
};