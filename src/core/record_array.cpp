#include "record_array.hpp"
#include <stdexcept>

RecordArray::RecordArray() : data_(nullptr), size_(0), capacity_(0) {}

RecordArray::RecordArray(std::uint32_t size, std::uint32_t capacity)
    : data_(capacity ? new Record[capacity] : nullptr), size_(size),
      capacity_(capacity) {}

RecordArray::~RecordArray() { delete[] data_; }

void RecordArray::add_record(Record record) {
  if (size_ == capacity_) {
    this->resize(capacity_ * 2);
  }
  data_[size_] = record;
    size_++;
}

// For now only allow new_capacity to be bigger than the current capaxity
void RecordArray::resize(std::uint32_t new_capacity) {
  if (new_capacity < capacity_) {
    throw std::runtime_error("attempt to resize record array to be smaller");
  }
  Record *new_data = new Record[new_capacity];
  for (std::size_t i = 0; i < size_; ++i) {
    new_data[i] = data_[i];
  }
  capacity_ = new_capacity;
  delete[] data_;
  data_ = new_data;
}

void RecordArray::delete_record(std::uint32_t index) {
  if (index >= size_) {
    throw std::invalid_argument("Invalid index");
  }

  data_[index] = data_[size_ - 1];
  size_--;
}
