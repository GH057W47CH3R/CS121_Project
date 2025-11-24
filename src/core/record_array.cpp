#include "record_array.hpp"
#include <stdexcept>

RecordArray::RecordArray() : data(nullptr), size(0), capacity(0) {}

RecordArray::~RecordArray() {
  delete[] data;
  data = nullptr;
  size = 0;
  capacity = 0;
}

void RecordArray::add_record(Record record) {
  if (this->size == this->capacity) {
    this->resize(this->capacity * 2);
  }
  this->data[size] = record;
}

// For now only allow new_capacity to be bigger than the current capaxity
void RecordArray::resize(std::uint32_t new_capacity) {
  if (new_capacity < this->capacity) {
    throw std::runtime_error("attempt to resize record array to be smaller");
  }
  Record *new_data = new Record[new_capacity];
  for (std::size_t i = 0; i < this->size; ++i) {
    new_data[i] = this->data[i];
  }
  this->capacity = new_capacity;
  delete[] this->data;
  this->data = new_data;
}

void RecordArray::delete_record(std::uint32_t index) {
  if (index >= size) {
    throw std::invalid_argument("Invalid index");
  }

  data[index] = data[size - 1];
  size--;
}
