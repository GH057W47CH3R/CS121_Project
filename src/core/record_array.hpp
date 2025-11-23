#pragma once
#include "record.hpp"
#include <cstdint>

struct RecordArray {
  Record *data;
  std::uint32_t size;
  std::uint32_t capacity;

  RecordArray();
  // RecordArray(std::uint32_t _capacity) {
  //   data = new Record[capacity];
  //   size = 0;
  //   capacity = _capacity;
  // }
  ~RecordArray();

  void push(Record);

  void resize(std::uint32_t);
};
