#pragma once
#include <string>

class Contact {
private:
    std::string name;
    std::string phoneNumber;
    std::string email;
    std::string address;

public:
    // Default constructor
    Contact() = default;

    Contact(const std::string& name,
        const std::string& phoneNumber,
        const std::string& email,
        const std::string& address);

    // Getters
    std::string getName() const { return name; }
    std::string getPhone() const { return phoneNumber; }
    std::string getEmail() const { return email; }
    std::string getAddress() const { return address; }
};
