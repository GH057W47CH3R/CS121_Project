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
    std::uint32_t next_id_;

public:
    AppState() : records_state_(RecordArray()), next_id_(0) {}
    void getValidate(const std::string& name, const std::string& address, const std::string& phone);
    void load_from_file(const fs::path& p);
    void save_to_file(const fs::path& p);
    void add_record_to_state(Record);
    void delete_record_from_state(std::uint32_t);
    void delete_record_by_name(const std::string& name);
    void sort_by_name();
    void sort_by_phone();
    RecordView search_by_name(const std::string& name) const;
    void list_all_records() const;
    std::uint32_t get_next_id() { return next_id_; }
    std::uint32_t num_records();
    const Record& record_at(std::uint32_t) const;
    RecordView select(const Predicate& pred) const {
        return view_match<const Record>(records_state_.data_, records_state_.size_,
            pred);
    }
    MutableRecordView select_mut(const Predicate& pred) {
        return view_match<Record>(records_state_.data_, records_state_.size_, pred);
    }
    std::uint32_t delete_by_pred(const Predicate& pred) {
        // Find all matching records
        MutableRecordView matches = select_mut(pred);
        std::uint32_t deleted_count = 0;

        // Delete each matching record by index
        // Since deleting shifts elements, delete from end to start
        for (int i = matches.size_ - 1; i >= 0; --i) {
            Record* rec = matches[i];
            // Find index of the record in records_state_
            std::uint32_t idx = static_cast<std::uint32_t>(rec - records_state_.data_);
            records_state_.delete_record(idx);
            deleted_count++;
        }
        return deleted_count;
    }
};
