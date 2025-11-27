#pragma once
#include "record.hpp"
#include <cstdint>

struct RecordArray {
  Record *data_;
  std::uint32_t size_;
  std::uint32_t capacity_;

  RecordArray();
  // RecordArray(std::uint32_t _capacity) {
  //   data = new Record[capacity];
  //   size = 0;
  //   capacity = _capacity;
  // }
  ~RecordArray();

  void push(Record);

  void resize(std::uint32_t);

  void add_record(Record);

  void delete_record(std::uint32_t index);
};
