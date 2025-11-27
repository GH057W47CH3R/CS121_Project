#pragma once
#include "record_array.hpp"
#include <filesystem>

namespace fs = std::filesystem;

struct AppState {
private:
  RecordArray records_state_;

public:
  void load_from_file(const fs::path &);
  void save_to_file(const fs::path &);
  void add_record_to_state(Record);
  void delete_record_from_state(std::uint32_t);
  void search_state(char[64]);
};
