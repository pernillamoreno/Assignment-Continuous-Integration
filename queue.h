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

    // Method to resize the queue
    bool resize(size_t newscope)
    {
        // If the new scope is less than 3, return false
        if (newscope < 3)
            return false;

        // If the new scope is the same as the current scope, return true
        if (newscope == scope)
            return true;

        // If the new scope is greater than the current scope
        if (newscope > scope)
        {
            size_t numToAdd = newscope - scope; // Calculate the number of nodes to add

            Node *previousNode = head; // Initialize pointer to node prevNode tail

            // Loop until the node prevNode tail is found
            while (previousNode->next != tail)
                previousNode = previousNode->next;

            // Add nodes to the queue
            for (size_t i = 0; i < numToAdd; ++i)
            {
                Node *node = static_cast<Node *>(memory.malloc(sizeof(Node))); // Allocate memory for a new node
                if (node == nullptr)
                {
                    // If memory allocation fails, deallocate previously allocated memory and return false
                    while (previousNode->next != tail)
                    {
                        Node *temp = previousNode->next;
                        previousNode->next = temp->next;
                        temp->~Node();
                        memory.free(temp);
                    }
                    return false;
                }

                // Construct a new node using placement new
                (void)new (node) Node;

                // Insert the new node into the queue
                node->next = previousNode->next;
                previousNode->next = node;
            }

            tail = previousNode->next; // Update tail pointer
        }
        else // If the new scope is less than the current scope
        {
            size_t nodesToRemoveCount = scope - newscope; // Calculate the number of nodes to remove

            Node *prevNode = head; // Initialize pointer to node prevNode head

            // Loop until the node prevNode head is found
            while (prevNode->next != tail)
                prevNode = prevNode->next;

            // Remove nodes from the queue
            for (size_t i = 0; i < nodesToRemoveCount; ++i)
            {
                Node *nodeToRemove = tail->next; // Get the node to remove
                tail->next = nodeToRemove->next; // Update next pointer of tail
                nodeToRemove->~Node();           // Call destructor for the node
                memory.free(nodeToRemove);       // Deallocate memory for the node
            }

            // If count is greater than the new scope, update count
            if (count > newscope)
                count = newscope;

            scope = newscope; // Update scope
        }

        return true; // Return true to indicate successful resizing
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
