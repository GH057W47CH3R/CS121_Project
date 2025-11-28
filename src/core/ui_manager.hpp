#include "app_state.hpp"
#include <optional>
#include <ostream>

enum class Role { Admin, ReadOnly };
enum class CommandType { Quit, Help, Insert, Edit, Select, Invalid };

class Command {
  CommandType type;
  std::optional<Predicate> pred;
  std::optional<Record> record;
};

class UIManager {
private:
  Role role_;
  AppState *app_;
  std::ostream &out_;
  std::istream &in_;
  // this should take some buffer or (str) to parse from as argument.
  Command parse_command();
  void handle_command(Command &com);

public:
  UIManager(Role, AppState *, std::ostream &, std::istream &);
  void ui_loop();
};
