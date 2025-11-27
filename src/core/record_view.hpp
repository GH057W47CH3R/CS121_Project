#include <cstdint>
#include <stdexcept>

template <typename T> struct PtrArray {
private:
  T **ptrs_;
  std::uint32_t size_;
  std::uint32_t capacity_;

public:
  PtrArray();
  explicit PtrArray(std::uint32_t capacity)
      : ptrs_(capacity ? new T *[capacity] : nullptr), size_(0),
        capacity_(capacity) {}

  ~PtrArray() { delete[] ptrs_; };

  void resize(std::uint32_t new_capacity) {
    if (new_capacity < capacity_) {
      throw std::runtime_error("attempt to resize record array to be smaller");
    }
    T **new_ptrs = new T *[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
      new_ptrs[i] = ptrs_[i];
    }
    capacity_ = new_capacity;
    delete[] ptrs_;
    ptrs_ = new_ptrs;
  }

  void push(T *ptr) {
    if (size_ == capacity_) {
      this->resize(capacity_ * 2);
    }
    ptrs_[size_] = ptr;
  }
};
