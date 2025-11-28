#pragma once
#include "record.hpp"
struct Predicate {
public:
  bool eval(const Record &) const;
};
