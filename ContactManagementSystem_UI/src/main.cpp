#include <iostream>
#include <limits>
#include <regex>
#include <cctype>
#include "../include/ContactManager.h"

using namespace std;

// Validation helpers

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
	ContactManager manager;
	int choice;
	do {
		displayMenu();
		choice = getValidatedMenuChoice();

		switch (choice) {
		case 1: {
			string name = getValidatedName("Enter Name: ");
			string phone = getValidatedPhone("Enter Phone Number: ");
			string email = getValidatedEmail("Enter Email: ");
			string address = getNonEmptyString("Enter Address: ");

			Contact contact(name, phone, email, address);
			manager.addContact(contact);

			cout << "\nContact Added Successfully!\n" << endl;
			break;
		}
		case 2: {
			string name;
			cout << "Enter Name of Contact to Delete: ";
			cin >> name;
			manager.deleteContact(name);
			cout << "\n";
			break;
		}
		case 3: {
			string name;
			cout << "Enter Name of Contact to Search: ";
			cin >> name;
			manager.searchContact(name);
			cout << "\n";
			break;
		}
		case 4:
			manager.displayContacts();
			cout << "\n";
			break;
	
		case 5: {
			string filename = "data/contacts.txt";
			manager.saveToFile(filename);
			cout << "\n Contacts saved to ";
			break;
		}
		case 6: {
			cout << "Load feature is coming soon!" << endl;
			break;
		}
		case 7:
			cout << "Exiting..." << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	} while (choice != 7);
	return 0;
}
