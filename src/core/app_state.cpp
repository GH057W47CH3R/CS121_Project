#include "app_state.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

void AppState::load_from_file(const fs::path &p) {
  std::error_code ec;

  fs::path ext = p.extension();

  if (ext != ".txt") {
    throw std::invalid_argument("the file must be a txt file.");
  }

  if (fs::exists(p, ec)) {
    std::ifstream in(p);
    if (!in) {
      throw std::runtime_error("Error loading input stream.");
    }
    char line[64];
    if (!in.getline(line, 64))
      throw std::runtime_error("Failed to read record count line.");
    std::uint32_t count = static_cast<std::uint32_t>(std::stoul(line));
    Record *loaded_records = new Record[count];
    // max bytes in line is 64
    unsigned int i = 0;
    while (in.getline(line, 64) && i < count) {
      int matched = std::sscanf(line, "%63[^\n]", loaded_records[i].name);
      if (matched != 1) {
        delete[] loaded_records;
        throw std::runtime_error("Failed to parse record line.");
      }
      i++;
    }
    // TODO: use a non-default constructor instead of this.
    records_state_.data_ = loaded_records;
    records_state_.size_ = count;
    records_state_.capacity_ = count;
  } else if (ec) {
    throw std::runtime_error("Error checking path: " + ec.message());
  } else {

    std::ofstream out(p);
    if (!out) {
      throw std::runtime_error("Failed to create file: " + p.string());
    }
  }
}

void AppState::save_to_file(const fs::path &p) {
  std::ofstream out(p);
  if (!out) {
    throw std::runtime_error("Failed to open file for writing");
  }

  out << records_state_.size_ << "\n";

  for (std::uint32_t i = 0; i < records_state_.size_; i++) {
    out << records_state_.data_[i].name << "\n";
  }

  out.close();
  std::cout << "Records saved to file: " << p << "\n";
}
