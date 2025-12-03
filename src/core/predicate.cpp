#include "predicate.hpp"
#include <cstring>
#include <sstream>
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

// This isnt a defensive parsing just for simplicity we require
// strict adherence to the format
Predicate parse_predicate(const std::string &pred_str) {
  Predicate pred;

  std::string col, op, val;
  std::istringstream string_stream(pred_str);
  string_stream >> col >> op >> val;

  if (col == "id") {
    pred.col_ = Col::Id;
    pred.is_string_ = false;
  } else if (col == "name") {
    pred.col_ = Col::Name;
    pred.is_string_ = true;
  } else if (col == "address") {
    pred.col_ = Col::Address;
    pred.is_string_ = true;
  } else if (col == "phone") {
    pred.col_ = Col::Phone;
    pred.is_string_ = true;
  } else {
    throw std::runtime_error("The column does not exist");
  }

  if (pred.is_string_) {
    if (op == "=") {
      pred.op_ = Op::Eq;
    } else if (op == "*") {
      pred.op_ = Op::SubStrEq;
    } else {
      throw std::runtime_error("Invalid operation");
    }
  } else {
    if (op == "=") {
      pred.op_ = Op::Eq;
    } else if (op == "<") {
      pred.op_ = Op::Lt;
    } else if (op == ">") {
      pred.op_ = Op::Gt;
    } else {
      throw std::runtime_error("Invalid operation");
    }
  }

  if (pred.is_string_) {
    pred.string_val_ = val;
  } else {
    pred.int_val_ = static_cast<std::uint32_t>(std::stoul(val));
  }

  return pred;
}
