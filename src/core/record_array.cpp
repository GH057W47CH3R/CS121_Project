#include "record_array.hpp"
#include <iostream>

RecordArray::RecordArray() : data(nullptr), size(0), capacity(0) {}

RecordArray::~RecordArray() {
  delete[] data;
  data = nullptr;
  size = 0;
  capacity = 0;
}

void RecordArray::push(Record record) {
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

void RecordArray::add_record(const char* newName) {
    Record* newArr = new Record[size + 1];

    for (std::uint32_t i = 0; i < size; i++) {
        newArr[i] = data[i];
    }

    std::strncpy(newArr[size].name, newName, 63);
    newArr[size].name[63] = '\0';

    delete[] data;

    data = newArr;
    size++;
}

void RecordArray::delete_record(std::uint32_t index) {
    if (index >= size) {
        std::cerr << "Invalid index\n";
        return;
    }
    
    data[index] = data[size - 1] ;   
    size--;
}

void RecordArray::save_records(const fs::path& p) {
    std::ofstream out(p);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << p << "\n";
        return;
    }

    out << size << "\n";

    for (std::uint32_t i = 0; i < size; i++) {
        out << data[i].name << "\n";
    }

    out.close(); 
    std::cout << "Records saved to file: " << p << "\n";
}
