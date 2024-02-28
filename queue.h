#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <stdexcept>
#include <cstddef>
#include <type_traits>
#include <algorithm>

// Interface for memory management
struct IMemory
{
    // Function to allocate memory
    virtual void *malloc(size_t size) = 0;
    // Function to free memory
    virtual void free(void *ptr) = 0;
    // Virtual destructor
    virtual ~IMemory() = default;
};

// Circular queue class template
template <typename T>
class circularQueue
{
    // Internal structure for a node in the queue
    struct Node
    {
        T data;       // Data stored in the node
        Node *next; // Pointer to the next node
    };

    Node *head{nullptr}; // Pointer to the first node
    Node *tail{nullptr}; // Pointer to the last node
    size_t stockroom;         // Maximum size of the queue
    size_t counter;        // Number of elements in the queue

public:
    // Deleted copy constructor and assignment operator to prevent copying
    circularQueue(const circularQueue &) = delete;
    circularQueue &operator=(const circularQueue &) = delete;

    // Constructor
    circularQueue(size_t size) : head(nullptr), tail(nullptr), stockroom(0), counter(0)
    {
        // Initialize the circular queue with a given size
        if (size > 2) // Ensure the size is at least 5
        {
            stockroom = size;
            // Create nodes for each element in the queue
            for (size_t i = 0; i < size; ++i)
            {
                // Allocate memory for a new node
                Node *newNode = new (std::nothrow) Node;
                

                if (newNode == nullptr)
                {
                    // If memory allocation fails, clear the queue and exit the loop
                    clear();
                    break;
                }

                newNode->next = nullptr; // Initialize the new node

                // Connect the new node to the queue
                if (head == nullptr)
                {
                    head = newNode;
                    tail = newNode;
                }
                else
                {
                    tail->next = newNode;
                    tail = newNode;
                }
            }

            tail = head; // Make the queue circular
        }
    }

    // Destructor
    ~circularQueue() { clear(); }

    // Move constructor
    circularQueue(circularQueue &&that) noexcept : head(that.head), tail(that.tail), stockroom(that.stockroom), counter(that.counter)
    {
        // Transfer ownership of resources from 'that' to the current object
        that.head = nullptr;
        that.tail = nullptr;
        that.stockroom = 0;
        that.counter = 0;
    }

    // Move assignment operator
    circularQueue &operator=(circularQueue &&that) noexcept
    {
        if (this != &that)
        {
            // Clear the current queue and transfer resources from 'that'
            clear();
            stockroom = that.stockroom;
            counter = that.counter;
            head = that.head;
            tail = that.tail;

            // Reset 'that'
            that.counter = 0;
            that.head = nullptr;
            that.tail = nullptr;
        }
        return *this;
    }

    // Function to write an element into the queue
    bool write(const T &data)
    {//ADD TAIL
        if (tail != nullptr)
        {

            // Store the data in the current tail node
            tail->data = data;
            // Move the tail pointer to the next node
            tail = tail->next;

            // Check if the queue is full
            if (stockroom== counter)
            {
                // If full, move the head pointer to the next node
                head = head->next;
            }
            else
            {
                // Otherwise, increment the counter
                ++counter;
            }
            return true;
        }
        return false;
    }

    // Function to read an element from the queue
    // Function to read data from the queue
    bool read(T &item)
    {
        if (head != nullptr) // Check if the head is not null
        {
            item = head->data; // Read data from the current head node
            head = head->next; // Move the head pointer to the next node
            --counter;         // Decrement the counter
            return true;       // Return true indicating successful read
        }
        return false; // Return false indicating failure to read
    }


    // Function to clear the queue
    void clear()
    {
        while (head != nullptr) // Loop until head becomes nullptr
        {
            Node *temp = head; // Store the current head in a temporary pointer
            head = head->next;   // Move the head pointer to the next node
            delete temp;         // Delete the node pointed to by temp
            if (head == tail)    // Check if head has reached the tail
            {
                break; // If so, break the loop
            }
        }
        head = tail = nullptr; // Set both head and tail to nullptr
        counter = 0;           // Reset the counter to 0
    }
    // Function to return the number of elements in the queue
    size_t count() { return counter; }

    // Function to check if the queue is full
    bool fullQueue() { return counter == stockroom; }

    // Function to calculate the average of elements in the queue
    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average()
    {
        // Output a message to indicate the start of the average calculation

        // Initialize the sum
        double sum = 0.0;
        // Pointer to traverse the queue
        Node *position = head;
        // Iterate through the queue and calculate the sum of elements
        for (size_t i = 0; i < counter; ++i)
        {
            sum += static_cast<double>(position->data);
            // Print out the current element being added to the sum

            position = position->next;
        }

        // Calculate the average
        double avg = (counter == 0) ? 0 : (sum / counter);

        // Return the calculated average
        return avg;
    }

    // Function to resize the queue
    void resize(size_t newSize)
    {
       

        if (newSize == stockroom)
        {

            
            // If the new size is equal to the current size, do nothing
            return;
        }

        if (newSize > 2)
        {
            // If the new size is greater than 2, adjust the queue size
            size_t oldElements = stockroom - newSize;
            Node *position = head;
            // Remove excess nodes from the head of the queue
            for (size_t i = 0; i < oldElements; ++i)
            {
                Node *temp = position;
                position = position->next;
                delete temp;
               
            }
            // Update the head and tail pointers and counter
            head = position;
            tail = head;
            counter = std::min(counter, newSize);
        }
        else
        {
            // If the new size is 2 or less, adjust the queue size accordingly
            size_t newElements = newSize - stockroom;
            Node *previously = tail;
            // Add new nodes to the tail of the queue
            for (size_t i = 0; i < newElements; ++i)
            {
                Node *newNode = new (std::nothrow) Node;
                if (newNode == nullptr)
                {
                    // If memory allocation fails, clear the queue and exit
                    clear();
                    break;
                }
                previously->next = newNode;
                previously = newNode;
            }
            // Make the queue circular
            previously->next = head;
        }
        // Update the memory size

        stockroom = newSize;
        
    }
};

#endif
