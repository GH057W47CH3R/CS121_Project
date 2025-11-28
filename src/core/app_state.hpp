#pragma once
#include "predicate.hpp"
#include "ptr_array.hpp"
#include "record_array.hpp"
#include <filesystem>

namespace fs = std::filesystem;

template <typename T>
PtrArray<T> view_match(T *data, std::uint32_t size, const Predicate &pred) {
  PtrArray<T> view;

  for (std::uint32_t i = 0; i < size; ++i) {
    if (pred.eval(data[i])) {
      view.push(&data[i]);
    }
  }

  return view;
}

using RecordView = PtrArray<const Record>;
using MutableRecordView = PtrArray<Record>;

struct AppState {
private:
  RecordArray records_state_;

public:
  void load_from_file(const fs::path &);
  void save_to_file(const fs::path &);
  void add_record_to_state(Record);
  void delete_record_from_state(std::uint32_t);
  RecordView select(const Predicate &pred) {
    return view_match<const Record>(records_state_.data_, records_state_.size_,
                                    pred);
  }
  MutableRecordView select_mut(const Predicate &pred) {
    return view_match<Record>(records_state_.data_, records_state_.size_, pred);
  }
};
