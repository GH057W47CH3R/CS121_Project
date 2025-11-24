#include "core/app_state.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  // ensure the second argument is a path
  if (argc != 2) {
    std::cerr << "usage: cms <file_path>\n";
    return 1;
  }
  fs::path p = argv[1];

  AppState app_state;
  app_state.load_from_file(p);
  // do stuff
  // TODO: Integrate UI Loop

  app_state.save_to_file(p);
  return 0;
}
