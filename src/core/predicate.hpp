#pragma once
#include "record.hpp"
// boolean operations
enum class Op { Eq, Ne };
enum class Col { Name };
struct Condition {
  Col col_;
  Op op_;
  char val_[64];
};

// this is just one condition for now its separated to allow for multiple
// conditions down the line
struct Predicate {
private:
  Condition cond_;

public:
  bool eval(const Record &) const;
};
