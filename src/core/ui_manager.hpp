#pragma once

#include "app_state.hpp"
#include <optional>
#include <iostream>

enum class Role { Admin, ReadOnly };
enum class CommandType { Quit, Help, Insert, Edit, Select, Invalid, EditById };

class Command {
public:
  CommandType type;
  // for commands like Select
  std::optional<Predicate> pred;
  // for commands like insert
  std::optional<Record> record;
};

class UIManager {
private:
  Role role_;
  // we use a pointer to AppState since UI manager may
  // be constructed before the connection is established
  AppState *app_;
  std::ostream &out_;
  std::istream &in_;

  // internal functions
  Predicate parse_predicate();
  // this should take some buffer or (str) to parse from as argument.
  Command parse_command();
  void handle_command(Command &com);

public:
  UIManager(Role, AppState *, std::ostream &, std::istream &);
  UIManager();
  void ui_loop();
};
