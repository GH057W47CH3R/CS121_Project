#include "core/record_array.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  // ensure the second argument is a path
  if (argc != 2) {
    std::cerr << "usage: cms <file_path>\n";
    return 1;
  }

  fs::path p = argv[1];
  std::error_code ec;
  fs::path ext = p.extension();

  if (ext != ".txt") {
    std::cerr << "the file must be a txt file.\n";
    return 1;
  }

  RecordArray record_arr;

  if (fs::exists(p, ec)) {
    std::ifstream in(p);
    if (!in) {
      std::cerr << "Error loading input stream.\n";
      return 1;
    }
    char line[64];
    if (!in.getline(line, 64))
      return 1;
    std::uint32_t count = static_cast<std::uint32_t>(std::stoul(line));
    std::cout << count << "\n";
    Record *loaded_record = new Record[count];
    // max bytes in line is 64
    unsigned int i = 0;
    while (in.getline(line, 64) && i < count) {
      int matched = std::sscanf(line, "%63[^\n]", loaded_record[i].name);
      i++;
    }
    record_arr.data = loaded_record;
  } else if (ec) {
    std::cerr << "Error checking path" << ec.message() << std::endl;
    return 1;
  } else {
    std::ofstream out(p);
    if (!out) {
      std::cerr << "Failed to create file: " << p << "\n";
      return 1;
    }
  }

  std::cout << "To add a name to cms type: add <name>" << std::endl;

  return 0;
}
