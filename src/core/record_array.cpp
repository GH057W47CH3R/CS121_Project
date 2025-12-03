#include "record_array.hpp"
#include <cstring>
#include <stdexcept>

// Default constructor
RecordArray::RecordArray() : data_(nullptr), size_(0), capacity_(0) {}

// Constructor creates an array with explicit size and capacity
// Size is current elements
// Capacity is max elements
RecordArray::RecordArray(std::uint32_t size, std::uint32_t capacity)
    : data_(capacity ? new Record[capacity] : nullptr), size_(size),
      capacity_(capacity) {}
// Destructor releases array
RecordArray::~RecordArray() { delete[] data_; }

void RecordArray::add_record(Record record) {
  // If the array is full, grow. Starts at 4 or doubles the existing capacity
  if (size_ == capacity_) {
    std::uint32_t new_cap = (capacity_ == 0 ? 4 : capacity_ * 2);
    resize(new_cap);
  }
  // Insert a new record at the end of array
  data_[size_] = record;
  ++size_;
}

void RecordArray::resize(std::uint32_t new_capacity) {
  if (new_capacity == capacity_)
    return;
  // Check for if new capacity will be too small for current array
  if (new_capacity < size_) {
    throw std::runtime_error("attempt to resize smaller than current size");
  }

  Record *new_data = nullptr;
  // Allocate new memory as long as new capacity is not 0
  if (new_capacity > 0) {
    new_data = new Record[new_capacity];

    // Copies existing records into new block, only copy if old data exists
    if (data_ != nullptr) {
      for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
      }
    }
  }
  // Free old storage and replace it
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
}

void RecordArray::delete_record(std::uint32_t index) {
  // Bounds check
  if (index >= size_) {
    throw std::invalid_argument("Invalid index");
  }
  // Removes element by overwriting it with last valid element
  data_[index] = data_[size_ - 1];
  size_--;
}

void RecordArray::serialize(std::ostream &out) const {
  // Then write each record in binary form
  for (std::uint32_t i = 0; i < size_; i++) {
    data_[i].serialize(out);
  }
}

void RecordArray::deserialize(std::istream &in, uint32_t count) {
  // Makes sure internal has exactly "count" capacity.
  resize(count);
  size_ = count;
  // Read each record from the binary stream
  for (std::uint32_t i = 0; i < size_; i++) {
    data_[i].deserialize(in);
    if (!in) {
      throw std::runtime_error(
          "Failed to read record data during deserialization.");
    }
  }
}
