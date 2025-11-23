#include "../include/Contact.h"

Contact::Contact(const std::string& name,
    const std::string& phoneNumber,
    const std::string& email,
    const std::string& address)
    : name(name), phoneNumber(phoneNumber), email(email), address(address) {
}
