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

      app_->add_record_to_state(from_string(name, address, phone));

      out_ << "Contact added successfully.\n";
    }

    // ---------- DELETE function -------------
    else if (command.rfind("DEL ", 0) == 0) {

      std::string target = command.substr(4);
      bool deleted = false;
      // #TODO DEL BY ID

      // maybe we'll add this for now commenting out
      // for (int i = 0; i < count; i++) {
      //   if (contacts[i].getName() == target) {
      //     for (int k = i; k < count - 1; k++)
      //       contacts[k] = contacts[k + 1];
      //     count--;
      //     deleted = true;
      //     break;
      //   }
      // }

      out_ << (deleted ? "Contact deleted.\n" : "No contact found.\n");
    }

    // ---------- SELECT function -------------
    else if (command.rfind("SELECT ", 0) == 0) {
      int i = 7; // skip "SELECT "
      std::string field, value;
      std::string pred_str = command.substr(7);
      Predicate p;
      try {
        Predicate p = parse_predicate(pred_str);
      } catch (const std::runtime_error &e) {
        out_ << "Parsing error: " << e.what() << "\n";
        continue;
      }
      RecordView selected_records = app_->select(p);
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

    // ---------- HELP function -------------
    else if (command == "-h") {
      out_ << "\n-------- Help Menu -------\n";
      out_ << "ADD \"name\" \"address\" \"phone\"\n";
      out_ << "DEL <name>\n";
      out_ << "SELECT name=<value>\n";
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
}
