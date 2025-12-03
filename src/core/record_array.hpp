#pragma once
#include "record.hpp"
#include <cstdint>

struct RecordArray {
  Record *data_;
  std::uint32_t size_;
  std::uint32_t capacity_;

  RecordArray();
  RecordArray(std::uint32_t size, std::uint32_t capacity);

  ~RecordArray();

  Record &operator[](std::uint32_t i) { return data_[i]; }

  const Record &operator[](std::uint32_t i) const { return data_[i]; }

  void resize(std::uint32_t);

  void add_record(Record);

  void delete_record(std::uint32_t index);
};
