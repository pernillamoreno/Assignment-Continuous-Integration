#include <stdexcept>
#include <cstddef>
#include <type_traits>
#include <algorithm>

// Interface for memory management
struct IMemory {
    virtual void *malloc(size_t size) = 0; // Allocate memory
    virtual void free(void *ptr) = 0; // Free memory
    virtual ~IMemory() = default; // Virtual destructor

};
template <typename T>
class circularQueue {
    // Internal structure for a node in the queue
    struct node_t {
        T data; // Data stored in the node
        node_t *next; // Pointer to the next node
    };

    // Private member variables
    node_t *head{nullptr}; // Pointer to the first node
    node_t *tail{nullptr}; // Pointer to the last node
    size_t memory; // Maximum size of the queue
    size_t counter; // Number of elements in the queue
public:
    // Deleted copy constructor and assignment operator to prevent copying
    circularQueue(const circularQueue &) = delete;
    circularQueue &operator=(const circularQueue &) = delete;


    circularQueue(IMemory &_memory) : memory{_memory}{}


     bool enqueue(const T &item)
     {
        bool status{false};

        return status;
     }
     size_t size(void)
    {
        return 0;
    }
     void clear(void)
    {
    }
     


      // Destructor
    ~circularQueue() { clear(); }
    };