// contributors: Kaleb
#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>

// Record is a struct for the data for one contact
struct Record {
  inline static constexpr std::size_t max_name_bytes = 64;
  inline static constexpr std::size_t max_address_bytes = 128;
  inline static constexpr std::size_t max_phone_bytes = 32;

  std::uint32_t id;

  char name[max_name_bytes];
  char address[max_address_bytes];
  char phone[max_phone_bytes];

  // Serialize the record by writing the raw bytes to an output stream
  void serialize(std::ostream &out) const {
    out.write(reinterpret_cast<const char *>(&id), sizeof(id));
    out.write(name, max_name_bytes);
    out.write(address, max_address_bytes);
    out.write(phone, max_phone_bytes);
  }
  // Deserialize the record by reading the raw bytes from an input stream
  void deserialize(std::istream &in) {
    in.read(reinterpret_cast<char *>(&id), sizeof(id));
    in.read(name, max_name_bytes);
    in.read(address, max_address_bytes);
    in.read(phone, max_phone_bytes);
  }
};
