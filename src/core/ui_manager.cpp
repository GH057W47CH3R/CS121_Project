#include "ui_manager.hpp"
#include "record.hpp"
#include <cstring>
#include <iostream>

// helper function to go from string to record
// all we have for now is name
Record from_string(std::string &n, std::string &a, std::string &p) {
  Record r;
  std::strncpy(r.name, n.c_str(), sizeof(r.name) - 1);
  std::strncpy(r.address, a.c_str(), sizeof(r.address) - 1);
  std::strncpy(r.phone, p.c_str(), sizeof(r.phone) - 1);
  return r;
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
      this->parse_predicate(pred_str);
      // #TODO we'll talk about it

      // Read field (until '=')
      while (i < (int)command.size() && command[i] != '=')
        field += command[i++];

      // if (i >= (int)command.size() || command[i] != '=') {
      //   std::cout << "Invalid format. Use: SELECT field=value\n";
      //   continue;
      // }

      // i++; // skip '='

      // // Read value (until end of line)
      // while (i < (int)command.size())
      //   value += command[i++];

      // // Trim spaces (manual, no <algorithm>)
      // auto trim = [](std::string &s) {
      //   // Trim leading spaces
      //   size_t start = 0;
      //   while (start < s.size() && isspace(s[start]))
      //     start++;
      //   s = s.substr(start);

      //   // Trim trailing spaces
      //   int end = (int)s.size() - 1;
      //   while (end >= 0 && isspace(s[end]))
      //     end--;
      //   s = s.substr(0, end + 1);
      // };

      // trim(field);
      // trim(value);

      // bool found = false;

      // for (int j = 0; j < count; j++) {
      //   std::string contactValue;

      //   if (field == "name")
      //     contactValue = contacts[j].getName();
      //   else if (field == "address")
      //     contactValue = contacts[j].getAddress();
      //   else if (field == "phone")
      //     contactValue = contacts[j].getPhone();
      //   else {
      //     std::cout << "Invalid field. Use: name, address, or phone\n";
      //     break;
      //   }

      //   // Manual case-insensitive search without <algorithm>
      //   // Convert both strings to lowercase manually
      //   std::string contactLower, valueLower;
      //   for (char c : contactValue) {
      //     if (c >= 'A' && c <= 'Z')
      //       contactLower += c + ('a' - 'A');
      //     else
      //       contactLower += c;
      //   }
      //   for (char c : value) {
      //     if (c >= 'A' && c <= 'Z')
      //       valueLower += c + ('a' - 'A');
      //     else
      //       valueLower += c;
      //   }

      //   // Check if value is a substring of contactValue
      //   if (contactLower.find(valueLower) != std::string::npos) {
      //     std::cout << "\n--- Match Found ---\n";
      //     std::cout << "Name: " << contacts[j].getName() << std::endl;
      //     std::cout << "Address: " << contacts[j].getAddress() << std::endl;
      //     std::cout << "Phone: " << contacts[j].getPhone() << std::endl;
      //     std::cout << "-------------------\n";
      //     found = true;
      //   }
      // }

      // if (!found) {
      //   std::cout << "No matching contacts.\n";
      // }
    }
    // -------- LIST function ---------------
    else if (command == "LIST") {
      for (std::uint32_t i = 0; i < app_->num_records(); i++) {
        const Record &cur_rec = app_->record_at(i);
        out_ << "Id: " << cur_rec.id << "\n";
        out_ << "Name: " << cur_rec.name << "\n";
        out_ << "Address: " << cur_rec.address << "\n";
        out_ << "Phone: " << cur_rec.phone << "\n\n";
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

// This isnt a defensive parsing just for simplicity we require
// strict adherence to the format
Predicate UIManager::parse_predicate(const std::string &pred_str) {
  Predicate pred;

  std::string col, op, val;
  std::istringstream string_stream(pred_str);
  string_stream >> col >> op >> val;

  if (col == "id") {
    pred.col_ = Col::Id;
    pred.is_string_ = false;
  } else if (col == "name") {
    pred.col_ = Col::Name;
    pred.is_string_ = true;
  } else if (col == "address") {
    pred.col_ = Col::Address;
    pred.is_string_ = true;
  } else if (col == "phone") {
    pred.col_ = Col::Phone;
    pred.is_string_ = true;
  } else {
    throw std::runtime_error("The column does not exist");
  }

  if (op == "=")
    pred.op_ = Op::Eq;
  else if (op == "<")
    pred.op_ = Op::Lt;
  else if (op == ">")
    pred.op_ = Op::Gt;
  else if (op == "*")
    pred.op_ = Op::SubStrEq;
  else {
    throw std::runtime_error("Invalid operation");
  }

  if (pred.is_string_) {
    pred.string_val_ = val;
  } else {
    pred.int_val_ = static_cast<std::uint32_t>(std::stoul(val));
  }

  return pred;
}
