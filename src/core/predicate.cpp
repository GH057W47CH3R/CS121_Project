#include "predicate.hpp"
#include <cstring>
#include <stdexcept>
bool Predicate::eval(const Record &r) const {
  switch (col_) {
  case Col::Name:
  case Col::Phone:
  case Col::Address: {
    const char *field = nullptr;
    switch (col_) {
    case Col::Name:
      field = r.name;
      break;
    case Col::Phone:
      field = r.phone;
      break;
    case Col::Address:
      field = r.address;
      break;
    default:
      throw std::runtime_error("This should not be reachable");
    }

    switch (op_) {
    case Op::Eq:
      return std::strcmp(field, string_val_.c_str()) == 0;
    case Op::SubStrEq:
      return std::strstr(field, string_val_.c_str()) != nullptr;
    default:
      throw std::runtime_error("This is not reachable");
    }
  }

  case Col::Id:
    switch (op_) {
    case Op::Eq:
      return r.id == int_val_;
    case Op::Lt:
      return r.id < int_val_;
    case Op::Gt:
      return r.id > int_val_;
    default:
      throw std::runtime_error("This should not be reachable");
    }

  default:
    throw std::runtime_error("This should not be reachable");
  }

  return true;
}
