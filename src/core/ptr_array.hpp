#pragma once
#include <cstdint>
#include <stdexcept>
#include <cstddef> // for std::size_t

//This is a dynamic array that stores pointers to T
template <typename T>
struct PtrArray {
public:
    T** ptrs_; //Raw array of T* pointers
    std::uint32_t size_; //Number of current elements
    std::uint32_t capacity_; //Number of max elements

public:
    //Default constructor is an empyy array with no size allocation
    PtrArray() : ptrs_(nullptr), size_(0), capacity_(0) {}

    //This constructs an array with T* pointers and allocates size, but does not initialize them
    explicit PtrArray(std::uint32_t capacity)
        : ptrs_(capacity ? new T*[capacity] : nullptr), size_(0), capacity_(capacity) {}
    //Destructor releases the pointer array but DOES NOT delete the objects being pointed to
    ~PtrArray() { delete[] ptrs_; }
    
    //Resizes the pointer array
    void resize(std::uint32_t new_capacity) {
        //If the new resized capacity is lower than current elements, throw
        if (new_capacity < size_) {
            throw std::runtime_error("attempt to resize PtrArray smaller than current size");
        }
        //Allocate storage for pointers
        T** new_ptrs = new T*[new_capacity];
        //Copy existing pointers into the newly sized array
        for (std::size_t i = 0; i < size_; ++i) {
            new_ptrs[i] = ptrs_[i];
        }
        //Release the old array
        delete[] ptrs_;
        ptrs_ = new_ptrs;
        capacity_ = new_capacity;
    }

    //Push a pointer into the array, will grow in size if necessary
    void push(T* ptr) {
        if (size_ == capacity_) {
            std::uint32_t new_cap = (capacity_ == 0 ? 4 : capacity_ * 2);
            resize(new_cap);
        }
        ptrs_[size_++] = ptr;
    }

    //Checks index bounds, throws if invalid index
    T* operator[](std::size_t index) const {
        if (index >= size_) throw std::out_of_range("PtrArray index out of range");
        return ptrs_[index];
    }
};
