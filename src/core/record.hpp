#pragma once
#include <cstddef>

// We will add to this
struct Record {
  inline static constexpr std::size_t max_name_bytes = 64;

  char name[max_name_bytes];
};

// # TODO add serialize deserialize functions. serialize could be a constructor
// deserialize should be a member function that outputs the record as cstring.
