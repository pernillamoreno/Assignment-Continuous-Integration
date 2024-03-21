#ifndef QUEUE_H // Header guard to prevent multiple inclusion
#define QUEUE_H

#include <iostream> // Standard input/output stream
#include <cstddef>  // Standard library definitions

using namespace std; // Using the standard namespace

// Interface for memory management
struct IMemory
{
    // Virtual method for allocating memory
    virtual void *malloc(size_t size) = 0;
    // Virtual method for freeing memory
    virtual void free(void *ptr) = 0;
    // Virtual destructor for interface
    virtual ~IMemory() = default;
};

// Template class for a circular queue
template <typename T>
class circleQueue
{
    // Node structure for the elements of the queue
    struct Node
    {
        T data;         // Data stored in the node
        Node *next{nullptr}; // Pointer to the next node in the queue
    };

    size_t count{0};    // Number of elements currently in the queue
    size_t scope{0};    // Maximum capacity of the queue
    IMemory &memory;    // Reference to a memory management object
    Node *head{nullptr}; // Pointer to the first node in the queue
    Node *tail{nullptr}; // Pointer to the last node in the queue

public:
    // Deleted copy constructor to prevent copying
    circleQueue(const circleQueue &other) = delete;
    // Deleted copy assignment operator to prevent copying
    circleQueue &operator=(const circleQueue &other) = delete;

    // Constructor to initialize the circular queue with a given scope and memory object
    circleQueue(size_t _scope, IMemory &_memory) : scope{_scope}, memory{_memory}
    {
        // Check if the scope is less than 3, throw an invalid argument exception
        if (scope < 3)
        {
            throw invalid_argument("The size must be at least 3");
        }

        // Initialize the queue by allocating memory for each node
        for (size_t i = 0; i < scope; ++i)
        {
            // Allocate memory for a new node using the provided memory object
            Node *newNode{static_cast<Node *>(memory.malloc(sizeof(Node)))};

            // If memory allocation fails, deallocate previously allocated memory and throw a bad allocation exception
            if (newNode == nullptr)
            {
                while (head != nullptr)
                {
                    tail = head;
                    head = head->next;
                    tail->~Node();
                    memory.free(tail);
                }
                throw bad_alloc();
            }

            // Construct a new node using placement new
            (void)new (newNode) Node;

            // Connect the new node to the queue
            if (i == 0)
            {
                head = tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }

        // Make the queue circular by connecting the last node to the first node
        tail->next = head;
        tail = head;
    }

    // Move constructor to efficiently transfer ownership of resources
    circleQueue(circleQueue &&that) noexcept : count{that.count}, scope{that.scope}, memory{that.memory}, head{that.head}, tail{that.tail}
    {
        // Reset the moved-from object
        that.count = 0;
        that.scope = 0;
        that.head = nullptr;
        that.tail = nullptr;
    }

    // Move assignment operator to efficiently transfer ownership of resources
    circleQueue &operator=(circleQueue &&that) noexcept
    {
        // Check for self-assignment
        if (this != &that)
        {
            // Deallocate memory for the existing nodes
            for (size_t i = 0; i < count; i++)
            {
                tail = head;
                head = head->next;
                tail->~Node();
                memory.free(tail);
            }

            // Transfer ownership of resources from the right-hand side to the left-hand side
            memory = that.memory;
            scope = that.scope;
            count = that.count;
            head = that.head;
            tail = that.tail;

            // Reset the moved-from object
            that.count = 0;
            that.scope = 0;
            that.head = nullptr;
            that.tail = nullptr;
        }

        return *this;
    }

    // Method to write data into the queue
    void write(const T &data)
    {
        tail->data = data;   // Write data to the current tail node
        tail = tail->next;   // Move tail to the next node

        // If the queue is full, move head to the next node
        if (isFull())
        {
            head = head->next;
        }
        else
        {
            count++; // Increment the count of elements in the queue
        }
    }

    // Method to read data from the queue
    T read()
    {
        T data{head->data}; // Read data from the current head node

        // If the queue is not empty, move head to the next node and decrement the count
        if (count > 0)
        {
            head = head->next;
            --count;
        }

        return data; // Return the read data
    }

    // Method to check if the queue is full
    bool isFull()
    {
        return count == scope; // Return true if the count equals the scope
    }

    // Method to get the count of elements in the queue
    size_t counter()
    {
        return count; // Return the count of elements
    }

    // Method to empty the queue by resetting count and tail
    void empty()
    {
        count = 0;    // Reset the count
        tail = head;  // Reset the tail to the head
    }

    // Method to calculate the average of the values in the queue
    template <typename U = T, typename = enable_if_t<is_arithmetic<U>::value>>
    double average(void)
    {
        double sum = 0.0;        // Initialize sum variable
        Node *current = head;    // Initialize current node pointer to head

        // Loop through the queue and calculate the sum of values
        for (size_t i = 0; i < count; ++i)
        {
            sum += static_cast<double>(current->data); // Add current data to sum
            current = current->next;                    // Move to the next node
        }

        // Return the average (sum divided by count), or 0 if count is 0 to avoid division by zero
        return ((count > 0) ? (sum / count) : sum);
    }

// scope = 10
// newscope = 15

// scope = 15
bool resize(size_t newscope)
{
    // Check if the new size is less than the minimum allowed size (3).
    // If it is, return false to indicate the resize operation is not allowed.
    if (newscope < 3)
        return false;

    // If the new size is the same as the current size, no resize is needed.
    // Return true to indicate success.
    if (newscope == scope)
        return true;

    // Case when the new size is larger than the current size.
    if (newscope > scope)
    {
        // Calculate how many new nodes need to be added.
        size_t numToAdd = newscope - scope;

        // Find the last node in the list (before the tail).
        Node *previousNode = head; 
        while (previousNode->next != tail)
        {
            previousNode = previousNode->next;
        }

        // Add the new nodes.
        for (size_t i = 0; i < numToAdd; ++i)
        {
            // Allocate memory for a new node.
            Node *node = static_cast<Node *>(memory.malloc(sizeof(Node)));
            // Check if memory allocation failed.
            if (node == nullptr)
            {
                // If allocation failed, deallocate any new nodes that were added.
                while (previousNode->next != tail)
                {
                    Node *temp = previousNode->next;
                    previousNode->next = temp->next;
                    temp->~Node(); // Explicitly call the destructor.
                    memory.free(temp); // Free the allocated memory.
                }
                return false;
            }

            // Use placement new to construct the node in the allocated memory.
            (void)new (node) Node;

            // Insert the new node into the list.
            node->next = previousNode->next;
            previousNode->next = node;
        }

        // Update the tail to point to the new last node.
        tail = previousNode->next; 
    }
    else // Case when the new size is smaller than the current size.
    {
        // Calculate how many nodes need to be removed.
        size_t nodesToRemoveCount = scope - newscope;

        // Find the node after which nodes will be removed.
        Node *previousNode = head; 
        while (previousNode->next != tail)
           previousNode = previousNode->next;

        // Remove the nodes.
        for (size_t i = 0; i < nodesToRemoveCount; ++i)
        {
            Node *nodeToRemove = tail->next; 
            tail->next = nodeToRemove->next; 
            nodeToRemove->~Node(); // Call the destructor for the node to be removed.
            memory.free(nodeToRemove); // Free the allocated memory.
        }

        // Adjust the count if it's greater than the new size.
        if (count > newscope)
            count = newscope;        
    }

    // Update the scope to the new size.
    scope = newscope; 

    // Return true to indicate success.
    return true; 
}


    // Destructor to deallocate memory for the nodes
    ~circleQueue()
    {
        // Deallocate memory for each node in the queue
        for (size_t i = 0; i < scope; i++)
        {
            tail = head;
            head = head->next;
            tail->~Node();
            memory.free(tail);
        }
    }
};

#endif // End of header guard
