#include "core/app_state.hpp"
#include "core/ui_manager.hpp"
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
  UIManager ui_manager(Role::ReadOnly, &app_state, std::cout, std::cin);
  try {
    app_state.load_from_file(p);
    ui_manager.ui_loop();
    app_state.save_to_file(p);
  } catch (const std::exception &e) {
    // #TODO fail gracefully when possible
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
