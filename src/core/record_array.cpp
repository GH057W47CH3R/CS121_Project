#include "record_array.hpp"

RecordArray::RecordArray() : data(nullptr), size(0), capacity(0) {}

RecordArray::~RecordArray() {
  delete[] data;
  data = nullptr;
  size = 0;
  capacity = 0;
}
