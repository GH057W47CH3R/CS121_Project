#include "ui_manager.hpp"
#include "record.hpp"
#include <cstring>
#include <iostream>

// helper function to go from string to record
// all we have for now is name
Record from_string(std::string &s) {
  Record r;
  std::strncpy(r.name, s.c_str(), sizeof(r.name) - 1);
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

      app_->add_record_to_state(from_string(name));

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
      // #TODO we'll talk about it

      // Read field (until '=')
      // while (i < (int)command.size() && command[i] != '=')
      //   field += command[i++];

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
        out_ << "Name: " << app_->record_at(i).get_name() << "\n";
        // out_ << "Address: " << contacts[i].getAddress() << "\n";
        // out_ << "Phone: " << contacts[i].getPhone() << "\n\n";
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

// Putting these here to get rid of linker errors for now.
Predicate UIManager::parse_predicate() { return Predicate{}; }

Command UIManager::parse_command() {
  Command cmd;
  cmd.type = CommandType::Invalid;
  return cmd;
}

void UIManager::handle_command(Command & /*com*/) {
  // intentionally left blank for now.
}
