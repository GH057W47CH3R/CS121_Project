#include "predicate.hpp"
#include <cstring>
#include <stdexcept>

bool Predicate::eval(const Record &r) {
  switch (col_) {
  case Col::Name:
    switch (op_) {
    case Op::Eq:
      // strcmp
      return strcmp(r.name, c.val_);
    case Op::Ne:
      // !strcmp
      return !strcmp(r.name, c.val_);
    default:
      throw std::runtime_error("This is not reachable");
    };
    break;
  default:
    throw std::runtime_error("This should not be reachable");
  }
  return true;
}
bool Predicate::eval(const Record &rec) const { return eval_cond(cond_, rec); }
