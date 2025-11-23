#pragma once
#include <vector>
#include <string>
#include "Contact.h"

// ContactManager class to manage a list of contacts
class ContactManager {
private:
    std::vector<Contact> contacts;

	// Public member functions for contact management
public:
    void addContact(const Contact& contact);
    void deleteContact(const std::string& name);
    void searchContact(const std::string& name) const;
    void displayContacts() const;

    // File I/O requirement
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
};
