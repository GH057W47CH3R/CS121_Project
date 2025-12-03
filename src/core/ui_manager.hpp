#pragma once

#include "app_state.hpp"
#include <iostream>

enum class Role { Admin, ReadOnly };
// Maybe go back to this. Katies choice. The benefit is mostly ergonomic and
// richer types
// enum class CommandType { Quit, Help, Insert, Edit, Select, Invalid, EditById
// };
//
// class Command {
// public:
//   CommandType type;
//   // for commands like Select
//   std::optional<Predicate> pred;
//   // for commands like insert
//   std::optional<Record> record;
// };

class UIManager {
private:
  Role role_;
  // we use a pointer to AppState since UI manager may
  // be constructed before the connection is established
  AppState *app_;
  std::ostream &out_;
  std::istream &in_;
  void print_record(const Record *);
  void print_record(const Record &);

public:
  UIManager(Role, AppState *, std::ostream &, std::istream &);
  void ui_loop();
};
