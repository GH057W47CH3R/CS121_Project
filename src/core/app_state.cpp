// This file implements the functions declared in app_state.hpp
// as well as a helper function to turn string references to a record.
#include "app_state.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

// helper function to go from string to record
// all we have for now is name
Record from_string(const std::string &n, const std::string &a,
                   const std::string &p) {
  Record r;
  std::strncpy(r.name, n.c_str(), sizeof(r.name) - 1);
  std::strncpy(r.address, a.c_str(), sizeof(r.address) - 1);
  std::strncpy(r.phone, p.c_str(), sizeof(r.phone) - 1);
  return r;
}

void AppState::edit_by_pred(Predicate &p, Col &to_edit, std::string &new_val) {
  MutableRecordView selected_records = this->select_mut(p);
  // ---- VALIDATE FIRST ----
  try {
    for (uint32_t i = 0; i < selected_records.size_; i++) {

      Record *rec = selected_records[i];

      if (to_edit == Col::Name)
        this->validate_fields(new_val, rec->address, rec->phone);

      else if (to_edit == Col::Address)
        this->validate_fields(rec->name, new_val, rec->phone);

      else if (to_edit == Col::Phone)
        this->validate_fields(rec->name, rec->address, new_val);
    }
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(std::string("EDIT failed: ") + e.what());
  }

  // ---- APPLY THE EDIT ----
  for (uint32_t i = 0; i < selected_records.size_; i++) {
    Record *rec = selected_records[i];

    switch (to_edit) {
    case Col::Name:
      std::strcpy(rec->name, new_val.c_str());
      break;
    case Col::Address:
      std::strcpy(rec->address, new_val.c_str());
      break;
    case Col::Phone:
      std::strcpy(rec->phone, new_val.c_str());
      break;
    default:
      throw std::runtime_error("This path cannot be reached");
    }
  }
}

void AppState::validate_fields(const std::string &name,
                               const std::string &address,
                               const std::string &phone) {
  // Name cannot be empty
  if (name.empty()) {
    throw std::runtime_error("Name cannot be empty.");
  }

  if (address.size() >= Record::max_address_bytes) {
    throw std::runtime_error("Address must be less than " +
                             std::to_string(Record::max_address_bytes) +
                             " bytes");
  }
  if (name.size() >= Record::max_name_bytes) {
    throw std::runtime_error("Name must be less than " +
                             std::to_string(Record::max_name_bytes) + " bytes");
  }

  // Phone must be (XXX) XXX-XXXX
  if (phone.size() != 14) {
    throw std::runtime_error(
        "Phone number must be in the format (XXX) XXX-XXXX.");
  }

  for (int i = 0; i < 14; ++i) {
    char c = phone[i];

    if ((i == 0 && c != '(') || (i == 4 && c != ')') || (i == 5 && c != ' ') ||
        (i == 9 && c != '-') ||
        ((i != 0 && i != 4 && i != 5 && i != 9) && !std::isdigit((c)))) {
      throw std::runtime_error(
          "Phone number must be in the format (XXX) XXX-XXXX.");
    }
  }

  // Address "Street, City, State ZIP"
  size_t comma1 = address.find(',');
  size_t comma2 = address.find(',', comma1 + 1);
  size_t space = address.find(' ', comma2 + 2);

  if (comma1 == std::string::npos || comma2 == std::string::npos ||
      space == std::string::npos) {
    throw std::runtime_error(
        "Address must be in the format 'Street, City, State ZIP'.");
  }

  std::string street = address.substr(0, comma1);
  std::string city = address.substr(comma1 + 2, comma2 - comma1 - 2);
  std::string state = address.substr(comma2 + 2, space - comma2 - 2);
  std::string zip = address.substr(space + 1);

  // Removing any leading/trailing spaces
  auto trim = [&](std::string &s) {
    while (!s.empty() && s[0] == ' ')
      s.erase(0, 1);
  };
  trim(city);
  trim(state);
  trim(zip);

  // State must be 2 uppercase letters
  if (state.size() != 2 || !std::isupper(state[0]) || !std::isupper(state[1])) {
    throw std::runtime_error(
        "State must be 2 uppercase letters. i.e CA, IL, GA...");

    // Zip must be 5 digits
    if (zip.size() != 5)
      throw std::runtime_error("ZIP code must be 5 digits.");

    for (char c : zip) {
      if (!std::isdigit(c)) {
        throw std::runtime_error("ZIP code must be 5 digits.");
      }
    }
  }
}

bool AppState::add_record_from_strings(const std::string &name,
                                       const std::string &address,
                                       const std::string &phone) {
  this->validate_fields(name, address, phone);
  Record record = from_string(name, address, phone);
  if (!this->contains_exact_match(record)) {
    this->add_record_to_state(record);
    return true;
  } else {
    return false;
  }
}

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
      delete_count++;
    } else {
      ++i;
    }
  }
  return delete_count;
}

bool AppState::contains_exact_match(const Record &r) const {
  for (std::uint32_t i = 0; i < this->records_state_.size_; ++i) {
    if (!strcmp(records_state_[i].phone, r.phone) &&
        !strcmp(records_state_[i].address, r.address) &&
        !strcmp(records_state_[i].name, r.name)) {
      return true;
    }
  }
  return false;
}
