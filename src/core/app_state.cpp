#include "app_state.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

// Loads all records from a binary (.bin) file
// ASSUMPTION: this is only ran at the start of the program
void AppState::load_from_file(const fs::path &p) {
  // Opens the file for reading in binary
  std::ifstream in(p, std::ios::binary);
  // If the file doesn't exist, this creates an empty file
  if (!in) {
    std::ofstream out(p, std::ios::binary);
    // sets internal state to empty
    return;
  }

  uint32_t count = 0;
  // This reads the first 4 bytes for the number of objects for the count

  if (!in.read(reinterpret_cast<char *>(&count), sizeof(count))) {
    throw std::runtime_error("DB file corrupt: failed to read count");
  }
  // This reads the next 4 bytes from the file for the next ID
  if (!in.read(reinterpret_cast<char *>(&next_id_), sizeof(next_id_))) {
    throw std::runtime_error("DB file corrupt: failed to read next id");
  }

  // RecordArray function will deserialize data if it exists
  records_state_.deserialize(in, count);
}

// Save all records to a binary (.bin) file
void AppState::save_to_file(const fs::path &p) {
  // Open file for writing(this will overwrite existing file)
  std::ofstream out(p, std::ios::binary);
  // Excetion for if the file fails to open
  if (!out) {
    throw std::runtime_error("Failed to open file for writing: " + p.string());
  }

  // The number of records is copied to count
  uint32_t count = records_state_.size_;
  // This writes the number of records into the file
  out.write(reinterpret_cast<char *>(&count), sizeof(count));
  // This writes the next ID into the file
  out.write(reinterpret_cast<char *>(&next_id_), sizeof(next_id_));

  // Serialize the RecordArray to file
  records_state_.serialize(out);
}

// Adds a new record to the RecordArray
void AppState::add_record_to_state(Record rec) {
  // Assigns an ID and increments to next one
  rec.id = next_id_;
  next_id_++;
  records_state_.add_record(rec);
}
// Deletes a record from the RecordArray
void AppState::delete_record_from_state(std::uint32_t index) {
  records_state_.delete_record(index);
}
// Provides public way to get size of records_state_
std::uint32_t AppState::num_records() { return records_state_.size_; }
// Provides public method to view a record
const Record &AppState::record_at(std::uint32_t i) const {
  return records_state_[i];
}

std::uint32_t AppState::delete_by_pred(const Predicate &pred) {
  std::uint32_t i = 0;
  std::uint32_t delete_count = 0;

  while (i < records_state_.size_) {
    if (pred.eval(records_state_[i])) {
      this->delete_record_from_state(i);
    } else {
      ++i;
    }
  }
  return delete_count;
}
