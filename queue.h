#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <type_traits>
#include <vector>

struct IMemory
{
    virtual void *malloc(size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual ~IMemory() = default;
};

template <typename T>
class Queue
{
    struct node_t
    {
        T data;
        node_t *next;
    };

    IMemory &memory;
    size_t count{0};
    node_t *head{nullptr};
    node_t *tail{nullptr};
    size_t max_size; // Added member to store maximum size of the queue

public:
    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;

    Queue(IMemory &_memory, size_t _max_size) : memory{_memory}, max_size{_max_size} {
        // Initialize the queue with max_size nodes
        for(size_t i = 0; i < max_size; ++i) {
            node_t *node = static_cast<node_t *>(memory.malloc(sizeof(node_t)));
            (void)new (node) node_t{0, nullptr}; // Initialize data with 0
            if(head == nullptr) {
                head = node;
                tail = head;
            } else {
                tail->next = node;
                tail = node;
            }
            ++count;
        }
    }

    Queue(Queue &&that) noexcept;

    Queue &operator=(Queue &&that) noexcept;

    bool enqueue(const T &item);

    bool dequeue(T &item);

    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic<U>::value>>
    double average(void);

    size_t size(void) { return count; }

    void clear(void);

    void resize(size_t new_size); // Function to resize the queue

    ~Queue() { clear(); }
};

/*template <typename T>
Queue<T>::Queue(Queue<T> &&that) noexcept : memory{that.memory}, count{that.count}, head{that.head}, tail{that.tail}
{
    that.count = 0;
    that.head = nullptr;
    that.tail = nullptr;
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue<T> &&that) noexcept
{
    if (this != &that)
    {
        clear();

        memory = that.memory;
        count = that.count;
        head = that.head;
        tail = that.tail;

        that.count = 0;
        that.head = nullptr;
        that.tail = nullptr;
    }

    return *this;
}

template <typename T>
bool Queue<T>::enqueue(const T &item)
{
    bool status{false};

    if(count < max_size) {
        node_t *temp = head;
        head = head->next;
        memory.free(temp);
        node_t *node{static_cast<node_t *>(memory.malloc(sizeof(node_t)))};

        if (node != nullptr)
        {
            // Placement new is used to construct an object in an allocated block of memory.
            (void)new (node) node_t{item, nullptr};

            if (head == nullptr)
            {
                head = node;
                tail = head;
            }
            else
            {
                tail->next = node;
                tail = node;
            }

            status = true;
            count++;
        }
    }

    return status;
}

template <typename T>
bool Queue<T>::dequeue(T &item)
{
    bool status{false};

    if (head != nullptr)
    {
        item = head->data;

        node_t *temp{head};
        head = head->next;
        temp->~node_t();
        memory.free(temp);

        if (head == nullptr)
        {
            tail = head;
        }

        status = true;
        count--;
    }

    return status;
}

template <typename T>
void Queue<T>::clear(void)
{
    while (head != nullptr)
    {
        tail = head;
        head = head->next;
        tail->~node_t();
        memory.free(tail);
    }

    count = 0;
    tail = head;
}

template <typename T>
void Queue<T>::resize(size_t new_size)
{
    if(new_size > max_size) {
        // Allocate additional nodes if new size is greater than current size
        for(size_t i = 0; i < new_size - max_size; ++i) {
            node_t *node{static_cast<node_t *>(memory.malloc(sizeof(node_t)))};
            (void)new (node) node_t{0, nullptr}; // Initialize data with 0
            tail->next = node;
            tail = node;
            ++count;
        }
        max_size = new_size;
    } else if(new_size < max_size) {
        // Deallocate excess nodes if new size is less than current size
        for(size_t i = 0; i < max_size - new_size; ++i) {
            node_t *temp = head;
            head = head->next;
            memory.free(temp);
            --count;
        }
        max_size = new_size;
    }
}

template <typename T>
template <typename, typename>
double Queue<T>::average(void)
{
    double value{0.0};

    node_t *cursor{head};

    for (size_t i = 0; i < count; i++)
    {
        value += static_cast<double>(cursor->data);
        cursor = cursor->next;
    }

    return ((count > 0) ? (value / count) : value);
}*/

#endif
