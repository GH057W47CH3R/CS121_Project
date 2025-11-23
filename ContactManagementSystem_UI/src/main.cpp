#include <iostream>
#include "../include/ContactManager.h"

using namespace std;

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
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice) {
		case 1: {
			string name, phone, email, address;
			cout << "Enter Name: ";
			getline(cin, name);
			cout << "Enter Phone Number: ";
			getline(cin, phone);
			cout << "Enter Email: ";
			getline(cin, email);
			cout << "Enter Address: ";
			getline(cin, address);
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