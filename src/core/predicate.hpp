// contributors: Ethan
//
// Defines the Predicate struct
// uses Op and Col as opposed to numbered values for readability
#pragma once
#include "record.hpp"
// boolean operations
enum class Op { Eq, SubStrEq, Gt, Lt };
enum class Col {
  Name,
  Id,
  Address,
  Phone,
};

// this is just one condition for now its separated to allow for multiple
// conditions down the line
struct Predicate {
public:
  Col col_;
  Op op_;
  bool is_string_;

  std::uint32_t int_val_;
  std::string string_val_;

  bool eval(const Record &) const;
};

Predicate parse_predicate(const std::string &);
