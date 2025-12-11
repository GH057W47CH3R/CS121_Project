#pragma once

#include "app_state.hpp"
#include <iostream>

class UIManager {
private:
  // we use a pointer to AppState since UI manager may
  // be constructed before the connection is established
  AppState *app_;
  // output stream
  std::ostream &out_;
  // input stream
  std::istream &in_;
  // print a record to the output stream
  void print_record(const Record *);
  void print_record(const Record &);

public:
  UIManager(AppState *, std::ostream &, std::istream &);
  void ui_loop();
};
