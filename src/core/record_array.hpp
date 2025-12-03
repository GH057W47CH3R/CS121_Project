#pragma once
#include "record.hpp"
#include <cstdint>

//This is a dynamically resizable array of the Record objects
//Has member functions for resizing array, adding, deleting,serializing,
//and deserializing records.
struct RecordArray {
  Record *data_;
  std::uint32_t size_;
  std::uint32_t capacity_;
  //Default constructor
  RecordArray();
  //Constructor that creates a RecordArray with the given size and capacity
  //Size is current elements, capacity is max elements
  explicit RecordArray(std::uint32_t size, std::uint32_t capacity);
  //Destructor
  ~RecordArray();

  //Resize array to new capacity
  void resize(std::uint32_t);
  //Append a new record
  void add_record(Record);
  //Delete a record
  void delete_record(std::uint32_t index);
  //Writes to a binary output stream
  void serialize(std::ostream& out) const;
  //Reads a record from a binary output stream
  void deserialize(std::istream& in, uint32_t count);
};
