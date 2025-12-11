#include "ui_manager.hpp"
#include "predicate.hpp"
#include "record.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>

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

UIManager::UIManager(AppState *app, std::ostream &out, std::istream &in)
    : app_(app), out_(out), in_(in) {}

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
        bool success = app_->add_record_from_strings(name, address, phone);
        if (success) {
          out_ << "Contact added succesfully.\n";
        } else {
          out_ << "Invalid ADD: There is an exact match already stored. \n";
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

      out_ << "Deleted " << num_deleted << " contacts\n";
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
      std::size_t last_space = pred_and_new.rfind('|');

      if (last_space == std::string::npos) {
        out_ << "Parsing error: EDIT requires: column match_op match_value "
                "new_value\n";
        continue;
      }

      std::string pred_str = pred_and_new.substr(0, last_space);
      std::string col_and_val = pred_and_new.substr(last_space + 1);
      Col to_edit;
      size_t eq_ind = col_and_val.rfind('=');
      std::string new_col_string = col_and_val.substr(0, eq_ind);
      std::string new_val =
          col_and_val.substr(eq_ind + 2, col_and_val.size() - (eq_ind + 2) - 1);
      if (new_col_string == "id") {
        to_edit = Col::Id;
      } else if (new_col_string == "address") {
        to_edit = Col::Address;
      } else if (new_col_string == "phone") {
        to_edit = Col::Phone;
      } else if (new_col_string == "name") {
        to_edit = Col::Name;
      } else {
        out_ << "Invalid EDIT: invalid column to edit";
        continue;
      }

      Predicate p{};
      try {
        p = parse_predicate(pred_str);
      } catch (const std::runtime_error &e) {
        out_ << "Parsing error: " << e.what() << "\n";
        continue;
      }

      if (to_edit == Col::Id) {
        out_ << "Cant edit ID\n";
        continue;
      }

      try {
        app_->edit_by_pred(p, to_edit, new_val);
      } catch (const std::runtime_error &e) {
        out_ << "EDIT failed: " << e.what() << "\n";
        continue;
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
      out_ << "Ex: ADD \"John\" \"123 Clarenc St, Atlanta, GA 12345\" \"(123) "
              "456-7890\"\n";
      out_ << "DEL <column> <match_op> <match_val>\n";
      out_ << "Ex: DEL id > 3\n";
      out_ << "Ex: DEL name = John\n";
      out_ << "SELECT <column> <match_op> <match_val>\n";
      out_ << "Ex: SELECT id = 5\n";
      out_ << "EDIT <column> <match_op> <match_val>|<new_col>=<new_val>\n";
      out_ << "Ex: EDIT address = 123 Clarenc St, Atlanta, GA "
              "12345|address=\"534 "
              "Clark St, Chicago, IL 36566\"";
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
