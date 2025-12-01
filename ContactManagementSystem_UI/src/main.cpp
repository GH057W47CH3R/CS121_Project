#include <iostream>
#include <limits>
#include <regex>
#include <cctype>
#include "../include/ContactManager.h"

using namespace std;

// Validate integer menu input
int getValidatedMenuChoice() {
    int choice;
    while (true) {
        cout << "Choose an option (1-7): ";
        if (cin >> choice && choice >= 1 && choice <= 7) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        // If input is invalid
        cout << "Invalid input. Please enter a number between 1 and 7.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Validate name input (letters and spaces only)
string getValidatedName(const string& prompt) {
    string name;
    regex pattern(R"(^[A-Za-z\s]+$)");

    while (true) {
        cout << prompt;
        getline(cin, name);

        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t") + 1);

        if (regex_match(name, pattern))
            return name;

        cout << "Invalid name. Use letters and spaces only.\n";
    }
}


// Validate that user enters non-empty text
string getNonEmptyString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        // Trim spaces
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (!input.empty())
            return input;

        cout << "Input cannot be empty. Try again.\n";
    }
}

// Validate email format
string getValidatedEmail(const string& prompt) {
    string email;
    regex pattern(R"((\w+)(\.?\w+)*@\w+\.\w+)");

    while (true) {
        cout << prompt;
        getline(cin, email);

        if (regex_match(email, pattern))
            return email;

        cout << "Invalid email format. Try again.\n";
    }
}


string getValidatedPhone(const string& prompt) {
    string phone;
    regex pattern(R"(^[\d\-\s\(\)]+$)");

    while (true) {
        cout << prompt;
        getline(cin, phone);

        if (regex_match(phone, pattern) && phone.length() >= 7 && phone.length() <= 15)
            return phone;

        cout << "Invalid phone number. Use digits, spaces, () or - only (7–15 chars).\n";
    }
}

// Function to display the main menu
void displayMenu() {
    cout << "Contact Management System" << endl;
    cout << "\n------------------------\n" << endl;
    cout << "1. Add Contact" << endl;
    cout << "2. Delete Contact (COMING SOON)" << endl;
    cout << "3. Search Contact (COMING SOON)" << endl;
    cout << "4. Display All Contacts" << endl;
    cout << "5. Save Contacts to File" << endl;
    cout << "6. Load Contacts from File (COMING SOON)" << endl;
    cout << "7. Exit\n" << endl;
    cout << "\n------------------------\n" << endl;
    cout << "Choose an option: ";
    cout << "\n------------------------\n" << endl;
    
}

// Main function to run the contact management.
int main() {
        Contact contacts[100];
        int count = 0;
        string command;

        cout << "Type -h for help.\n";

        while (true) {
            cout << "$ ";
            getline(cin, command);

            if (command.rfind("ADD ", 0) == 0) {

                int i = 4; // skip "ADD "
                string name, address, phone;

                // ---- READ NAME ----
                while (i < (int)command.size() && command[i] != ' ') {
                    name += command[i++];
                }

                if (i >= (int)command.size()) {
                    cout << "Missing fields. Use: ADD <name> <address> <phone>\n";
                    continue;
                }

                while (i < (int)command.size() && command[i] == ' ') i++;

                // ---- READ ADDRESS ----
                while (i < (int)command.size() && command[i] != ' ') {
                    address += command[i++];
                }

                if (i >= (int)command.size()) {
                    cout << "Missing phone.\n";
                    continue;
                }

                while (i < (int)command.size() && command[i] == ' ') i++;

                // ---- READ PHONE ----
                while (i < (int)command.size() && command[i] != ' ') {
                    phone += command[i++];
                }

                // STORE CONTACT
                contacts[count] = Contact(name, phone, "", address);
                count++;

                cout << "Contact added successfully.\n";
            }
            else if (command.rfind("DEL ", 0) == 0) {

                string target = command.substr(4);

                bool deleted = false;

                for (int i = 0; i < count; i++) {
                    if (contacts[i].getName() == target) {

                        // shift left
                        for (int k = i; k < count - 1; k++) {
                            contacts[k] = contacts[k + 1];
                        }

                        count--;
                        cout << "Contact deleted successfully.\n";
                        deleted = true;
                        break;
                    }
                }

                if (!deleted) {
                    cout << "No contact with that name found.\n";
                }
            }

            else if (command.rfind("SELECT ", 0) == 0) {

                int i = 7; // skip "SELECT "
                string field, value;

                // ---- READ FIELD (until '=') ----
                while (i < (int)command.size() && command[i] != '=') {
                    field += command[i++];
                }

                if (i >= (int)command.size() || command[i] != '=') {
                    cout << "Invalid format. Use: SELECT name=VALUE\n";
                    continue;
                }

                i++; // skip '='

                // ---- READ VALUE (until end of line) ----
                while (i < (int)command.size()) {
                    value += command[i++];
                }

                bool found = false;

                // ---- SEARCH LOOP ----
                for (int j = 0; j < count; j++) {

                    if (field == "name" && contacts[j].getName() == value) {
                        cout << "\n--- Match Found ---\n";
                        cout << "Name: " << contacts[j].getName() << endl;
                        cout << "Address: " << contacts[j].getAddress() << endl;
                        cout << "Phone: " << contacts[j].getPhone() << endl;
                        cout << "-------------------\n";
                        found = true;
                    }
                    else if (field == "address" && contacts[j].getAddress() == value) {
                        cout << "\n--- Match Found ---\n";
                        cout << "Name: " << contacts[j].getName() << endl;
                        cout << "Address: " << contacts[j].getAddress() << endl;
                        cout << "Phone: " << contacts[j].getPhone() << endl;
                        cout << "-------------------\n";
                        found = true;
                    }
                    else if (field == "phone" && contacts[j].getPhone() == value) {
                        cout << "\n--- Match Found ---\n";
                        cout << "Name: " << contacts[j].getName() << endl;
                        cout << "Address: " << contacts[j].getAddress() << endl;
                        cout << "Phone: " << contacts[j].getPhone() << endl;
                        cout << "-------------------\n";
                        found = true;
                    }
                }

                if (!found) {
                    cout << "No matching contacts found.\n";
                }

            }

            else if (command == "LIST") {
                for (int i = 0; i < count; i++) {
                    cout << "Name: " << contacts[i].getName() << endl;
                    cout << "Address: " << contacts[i].getAddress() << endl;
                    cout << "Phone: " << contacts[i].getPhone() << endl;
                }
            }
            else if (command == "-h") {
                cout << "\n-------- Help Menu -------\n";
                cout << "ADD <name> <address> <phone> : Add a new contact\n";
                cout << "SELECT name=<value>          : Search for a contact by name\n";
                cout << "DEL <name>                   : Delete a contact by name\n";
                cout << "LIST                         : List all contacts\n";
                cout << "QUIT                         : Exit the program\n";
                cout << "---------------------------\n";
            }
            else if (command == "QUIT") {
                break;
            }
            else {
                cout << "Unknown command.\n";
            }
        }
        return 0;
}
