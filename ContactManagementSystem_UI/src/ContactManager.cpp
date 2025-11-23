#include "../include/ContactManager.h"
#include <iostream>
#include <fstream>

// Adds a new contact to the contact list
void ContactManager::addContact(const Contact& contact) {
	contacts.push_back(contact);
}

// Deletes a contact by name (functionality not yet implemented)
void ContactManager::deleteContact(const std::string& name) {
	std::cout << "Delete feature is coming soon!" << std::endl;
}

// Searches for a contact by name (functionality not yet implemented)
void ContactManager::searchContact(const std::string& name) const {
    std::cout << "Search feature is coming soon!" << std::endl;
}

// Displays all contacts in the contact list
void ContactManager::displayContacts() const {
    if (contacts.empty()) {
        std::cout << "No contacts to display.\n";
        return;
    }
    std::cout << "\n-------- Your Contancts List --------\n";
    for(const auto& contact : contacts) {
        std::cout << "Name: " << contact.getName() << std::endl;
        std::cout << "Phone: " << contact.getPhone() << std::endl;
        std::cout << "Email: " << contact.getEmail() << std::endl;
        std::cout << "Address: " << contact.getAddress() << std::endl;
        std::cout << "------------------------" << std::endl;
	}
}

// Loads contacts from a file (functionality not yet implemented)
void ContactManager::loadFromFile(const std::string& filename) {
	std::cout << "Load feature is coming soon!" << std::endl;
}

// Saves contacts to a file
void ContactManager::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
	}
    for (const auto& contact : contacts) {
        file << contact.getName() << ","
             << contact.getPhone() << ","
             << contact.getEmail() << ","
			<< contact.getAddress() << "\n";
    }
    file.close();
	std::cout << "Contacts saved to " << filename << std::endl;
}
