// contributors: Ethan, Kaleb, Katie
#pragma once
#include "predicate.hpp"
#include "ptr_array.hpp"
#include "record_array.hpp"
#include <filesystem>

namespace fs = std::filesystem;

// Ethan
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
  std::uint32_t next_id_;

  void add_record_to_state(Record);
  void delete_record_from_state(std::uint32_t);
  bool contains_exact_match(const Record &) const;
  MutableRecordView select_mut(const Predicate &pred) {
    return view_match<Record>(records_state_.data_, records_state_.size_, pred);
  }

public:
  AppState() : records_state_(RecordArray()), next_id_(0) {}
  void validate_fields(const std::string &name, const std::string &address,
                       const std::string &phone);
  void load_from_file(const fs::path &p);
  void save_to_file(const fs::path &p);
  std::uint32_t delete_by_pred(const Predicate &pred);
  std::uint32_t get_next_id() { return next_id_; }
  std::uint32_t num_records();
  const Record &record_at(std::uint32_t) const;
  bool add_record_from_strings(const std::string &name,
                               const std::string &address,
                               const std::string &phone);
  RecordView select(const Predicate &pred) const {
    return view_match<const Record>(records_state_.data_, records_state_.size_,
                                    pred);
  }
  void edit_by_pred(Predicate &, Col &, std::string &);
};
