#include "ui_manager.hpp"
#include "predicate.hpp"
#include "record.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>

// helper function to go from string to record
// all we have for now is name
Record from_string(std::string &n, std::string &a, std::string &p) {
  Record r;
  std::strncpy(r.name, n.c_str(), sizeof(r.name) - 1);
  std::strncpy(r.address, a.c_str(), sizeof(r.address) - 1);
  std::strncpy(r.phone, p.c_str(), sizeof(r.phone) - 1);
  return r;
}

void UIManager::print_record(const Record *r) {
  out_ << "Id: " << r->id << "\n";
  out_ << "Name: " << r->name << "\n";
  out_ << "Address: " << r->address << "\n";
  out_ << "Phone: " << r->phone << "\n\n";
}

void UIManager::print_record(const Record &r) {
  out_ << "Id: " << r.id << "\n";
  out_ << "Name: " << r.name << "\n";
  out_ << "Address: " << r.address << "\n";
  out_ << "Phone: " << r.phone << "\n\n";
}

UIManager::UIManager(Role role, AppState *app, std::ostream &out,
                     std::istream &in)
    : role_(role), app_(app), out_(out), in_(in) {}

void UIManager::ui_loop() {

  out_ << "Welcome to the Contact Management System!\n";
  out_ << "Type -h for help.\n";

  std::string command;

  while (true) {
    out_ << "$ ";
    std::getline(in_, command);

    // --------- ADD function --------------
    if (command.rfind("ADD ", 0) == 0) {

      int i = 4; // skip "ADD "
      std::string name, address, phone;

      auto readQuoted = [&](std::string &out) {
        while (i < command.size() && command[i] == ' ')
          i++;

        if (i >= command.size() || command[i] != '"')
          return false;

        i++;

        while (i < command.size() && command[i] != '"')
          out += command[i++];

        if (i >= command.size() || command[i] != '"')
          return false;

        i++;
        return true;
      };

      if (!readQuoted(name) || !readQuoted(address) || !readQuoted(phone)) {
        out_ << "Invalid format. Use: ADD \"name\" \"address\" \"phone\"\n";
        continue;
      }

      try {
        app_->getValidate(name, address, phone);
        Record record = from_string(name, address, phone);
        if (!app_->contains_exact_match(record)) {
          app_->add_record_to_state(record);
          out_ << "Contact added successfully.\n";
        } else {
          out_ << "Failed to add: There is an exact match of the record "
                  "already stored.\n";
        }
      } catch (const std::runtime_error &e) {
        out_ << "Invalid ADD: " << e.what() << "\n";
      }
    }
    // ---------- DELETE function -------------
    else if (command.rfind("DEL ", 0) == 0) {
      std::string pred_str = command.substr(4);
      Predicate p{};
      // TODO seperate this out into its own function
      try {
        p = parse_predicate(pred_str);
      } catch (const std::runtime_error &e) {
        out_ << "Parsing error: " << e.what() << "\n";
        continue;
      }
      std::uint32_t num_deleted = app_->delete_by_pred(p);

      out_ << "Deleted " << num_deleted << "contacts\n";
    }

    // ---------- SELECT function -------------
    else if (command.rfind("SELECT ", 0) == 0) {
      std::string pred_str = command.substr(7);
      Predicate p{};
      try {
        p = parse_predicate(pred_str);
      } catch (const std::runtime_error &e) {
        out_ << "Parsing error: " << e.what() << "\n";
        continue;
      }

      RecordView selected_records = app_->select(p);

      // Print count before listing
      out_ << "\n(" << selected_records.size_ << " contact"
           << (selected_records.size_ == 1 ? "" : "s") << " match!)\n";

      for (std::uint32_t i = 0; i < selected_records.size_; i++) {
        this->print_record(selected_records[i]);
      }
    }

    // -------- LIST function ---------------
    else if (command == "LIST") {
      for (std::uint32_t i = 0; i < app_->num_records(); i++) {
        const Record &cur_rec = app_->record_at(i);
        this->print_record(cur_rec);
      }
    }
    // ---------- EDIT function -------------
    else if (command.rfind("EDIT ", 0) == 0) {

      std::string pred_and_new = command.substr(5);
      std::size_t last_space = pred_and_new.find_last_of(' ');

      if (last_space == std::string::npos) {
        out_ << "Parsing error: EDIT requires: column match_op match_value "
                "new_value\n";
        continue;
      }

      std::string pred_str = pred_and_new.substr(0, last_space);
      std::string new_val = pred_and_new.substr(last_space + 1);

      Predicate p{};
      try {
        p = parse_predicate(pred_str);
      } catch (const std::runtime_error &e) {
        out_ << "Parsing error: " << e.what() << "\n";
        continue;
      }

      if (p.col_ == Col::Id) {
        out_ << "Cant edit ID\n";
        continue;
      }

      MutableRecordView selected_records = app_->select_mut(p);

      // ---- VALIDATE FIRST ----
      try {
        for (uint32_t i = 0; i < selected_records.size_; i++) {

          Record *rec = selected_records[i];

          if (p.col_ == Col::Name)
            app_->getValidate(new_val, rec->address, rec->phone);

          else if (p.col_ == Col::Address)
            app_->getValidate(rec->name, new_val, rec->phone);

          else if (p.col_ == Col::Phone)
            app_->getValidate(rec->name, rec->address, new_val);
        }
      } catch (const std::runtime_error &e) {
        out_ << "EDIT failed: " << e.what() << "\n";
        continue;
      }

      // ---- APPLY THE EDIT ----
      for (uint32_t i = 0; i < selected_records.size_; i++) {
        Record *rec = selected_records[i];

        switch (p.col_) {
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

      out_ << "EDIT successful.\n";
    }

    // ----- PREFIX SEARCH: *a, *b, *c -----
    else if (!command.empty() && command[0] == '*') {
      if (command.size() < 2) {
        out_ << "Invalid prefix search. Use *(letter). (i.e: *a, *b, *c)\n";
        continue;
      }

      char prefix = std::tolower(command[1]);
      bool found = false;

      out_ << "\n----- Contacts starting with '" << prefix << "' -----\n";

      // Count how many match the prefix
      int count = 0;
      for (uint32_t i = 0; i < app_->num_records(); i++) {
        const Record &cur = app_->record_at(i);
        if (std::tolower(cur.name[0]) == prefix) {
          count++;
        }
      }

      if (count == 0) {
        out_ << "No contacts found starting with '" << prefix << "'.\n";
        continue;
      }

      //  Make a temp array to hold matches
      Record *matches = new Record[count];
      int index = 0;

      for (uint32_t i = 0; i < app_->num_records(); i++) {
        const Record &cur = app_->record_at(i);
        if (std::tolower(cur.name[0]) == prefix) {
          std::memcpy(&matches[index], &cur, sizeof(Record));
          index++;
        }
      }

      // Sort alphabetically with simple bubble sort
      for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
          if (std::strcmp(matches[j].name, matches[j + 1].name) > 0) {
            Record temp = matches[j];
            matches[j] = matches[j + 1];
            matches[j + 1] = temp;
          }
        }
      }

      // Print results
      for (int i = 0; i < count; i++) {
        this->print_record(matches[i]);
      }

      delete[] matches;
    }

    // ---------- HELP function -------------
    else if (command == "-h") {
      out_ << "\n-------- Help Menu -------\n";
      out_ << "ADD \"name\" \"address\" \"phone\"\n";
      out_ << "DEL <column> <match_op> <match_val>\n";
      out_ << "SELECT <column> <match_op> <match_val>\n";
      out_ << "EDIT <column> <match_op> <match_val> <new_val>\n";
      out_ << "LIST\n";
      out_ << "QUIT\n";
      out_ << "-------------------------\n";
    }

    // ---------- QUIT function -------------
    else if (command == "QUIT") {
      out_ << "Goodbye!\n";
      break;
    }

    else {
      out_ << "Unknown command.\n";
    }
  }
};
