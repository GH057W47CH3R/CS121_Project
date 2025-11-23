# Contact Management System — Project Documentation

## 1. Project Purpose
The purpose of this project is to create a Contact Management System in C++ that allows a user to store, view, and manage contact information.  
This project demonstrates the use of:
- Structs  
- Modular programming  
- File Input/Output  
- Basic menu-driven UI  
- Clean project organization  

---

## 2. Problem Statement
Users need a simple way to save contact information (name, phone, email, address) and store it persistently so the information is not lost when the program closes.  
This prototype implements the core structure of the final system.

---

## 3. Folder Structure

ContactManagementUI/
│
├── include/
│ └── Contact.h
│ └── ContactManager.h
│
├── src/
│ └── Contact.cpp
│ └── ContactManager.cpp
│ └── main.cpp
│
├── data/
│ └── contacts.txt (created when saving)
│
└── Documentation.md

- Files are separated logically
- Classes/structs live in 'include/'
- Implementations live in 'src/'
- File I/O are stored in 'data/'

----

## 4. Classes
### **Contact**
- Stores fields: 'name', 'phone', 'email', 'address'
- Uses constructors & string formatting helpers

### **ContactManager**
- Manages a list of contacts
- Has functions for
	- 'addContact()'
	- 'saveToFile()'
	- 'loadToFile()" "COMING SOON"
	- 'searchContact()' "COMING SOON"
	- 'deleteContact()' "COMING SOON"

All class designs match those in the document.

----

## 5. Functions
### **Completed**
- `addContact()`: adds a new contact  
- `saveToFile()` : writes all contacts to `data/contacts.txt`  
- `displayMenu()` : simple UI menu  
- `main()` : loops until user chooses Exit 
 
### **Partially Implemented**
- `searchContact()` : currently displays "Coming soon!"  
- `deleteContact()`: currently displays "Coming soon!"  
- `loadFromFile()` : planned for final version  

Placeholders are used but the structure is complete.

## 6. File I/O Example
This prototype includes a fully working save example.

### Save to File
- Writes all current contacts to '/data/contacts.txt'
- This demonstrates the use of:
	= file streams
	- error checking
	- path handling
- Commented for clarity

### Not Yet Implemented: Load from File
- A placeholder function exists
- Will be completed in the final submission

----

## 7. Third-Party Libraries

No Third-Party Libraries are used in this section of the project so far.

----

## 8. Comments

- All .cpp files include header comments
- Functions include inline comments explaining instructions/logic.
- File I/O contains more detailed explanatory comments.

----

## 9. Current Features
- Add contact
- Saves contact to file
- Basic text UI
- File & folder organization
- All core classes defined

----

## 10. Planned Features
These will be implemented later:
- Load contacts from file
- Search for contacts
- Delete contact(s)
- Input validation
- Improved UI formatting

----

## 11. Ethical Considerations
- No real personal data is used in this prototype
- All contacts are created for testing are functional
- No external code or third-party libraries were used.
- All work follows the course academic integrity guidelines.
- No unauthorized collabs occurred.

----
## 12. Version Note
This is an early prototype submission.
Functionally is intentionally limited but is fully structured to support later expansion.