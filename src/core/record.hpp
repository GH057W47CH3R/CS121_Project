#pragma once
#include <cstddef>

// We will add to this
struct Record {
  inline static constexpr std::size_t max_name_bytes = 64;

  char name[max_name_bytes];

  const char *get_name() const { return name; }

  //Serialize the record by writing the raw bytes to an output stream
  void serialize(std::ostream& out) const{
    out.write(name, max_name_bytes);
  }
  //Deserialize the record by reading the raw bytes from an input stream
  void deserialize(std::istream& in) {
    in.read(name, max_name_bytes);
}

};
