#include "signup.h"
#include <cctype>
#include <sstream>

// Customer class implementation
Customer::Customer() : isLocked(false), failedAttempts(0), lockTime(0) {}

string Customer::hashPassword(const string& password) {
    // Simple hashing technique using XOR and bit manipulation
    // This is a basic demonstration - for production use, consider stronger algorithms
    
    unsigned int hash = 5381; // Initial prime number
    
    // XOR and shift operations to create hash
    for (char c : password) {
        hash = ((hash << 5) + hash) ^ c; // hash * 33 XOR c
    }
    
    // Convert to hexadecimal string
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

void Customer::setPassword(string pass) {
    passwordHash = hashPassword(pass);
}

bool Customer::verifyPassword(string pass) const {
    string inputHash = hashPassword(pass);
    return passwordHash == inputHash;
}

bool Customer::isAccountLocked() const {
    if (isLocked) {
        time_t currentTime = time(nullptr);
        double hoursElapsed = difftime(currentTime, lockTime) / 3600.0;
        if (hoursElapsed >= 24) {
            return false; // Lock expired
        }
        return true;
    }
    return false;
}

void Customer::incrementFailedAttempts() {
    failedAttempts++;
    if (failedAttempts >= 3) {
        isLocked = true;
        lockTime = time(nullptr);
    }
}

void Customer::resetFailedAttempts() {
    failedAttempts = 0;
    isLocked = false;
}

void Customer::setBankAccountNo(string accNo) {
    bankAccountNo = accNo;
}

string Customer::getBankAccountNo() const {
    return bankAccountNo;
}

// File output operator for Customer
ofstream& operator<<(ofstream& ofs, const Customer& cust) {
    ofs << cust.username << "\n";
    ofs << cust.name << "\n";
    ofs << cust.phno << "\n";
    ofs << cust.bankAccountNo << "\n";
    ofs << cust.passwordHash << "\n";  // Save hashed password
    ofs << cust.failedAttempts << "\n";
    ofs << cust.isLocked << "\n";
    ofs << cust.lockTime << "\n";
    return ofs;
}

// File input operator for Customer
ifstream& operator>>(ifstream& ifs, Customer& cust) {
    getline(ifs, cust.username);
    getline(ifs, cust.name);
    getline(ifs, cust.phno);
    
    string bankAcc;
    getline(ifs, bankAcc);
    cust.setBankAccountNo(bankAcc);
    
    // Read the stored password hash
    getline(ifs, cust.passwordHash);
    
    string attemptsStr;
    getline(ifs, attemptsStr);
    cust.failedAttempts = stoi(attemptsStr);
    
    string lockedStr;
    getline(ifs, lockedStr);
    cust.isLocked = (lockedStr == "1");
    
    string timeStr;
    getline(ifs, timeStr);
    cust.lockTime = stol(timeStr);
    
    return ifs;
}

// UserRegistration class implementation
UserRegistration::UserRegistration() {
    loadFromFile();
}

UserRegistration::~UserRegistration() {
    saveToFile();
}

bool UserRegistration::isValidPassword(const string& password) const {
    if (password.length() < 8) return false;
    
    bool hasUpper = false, hasLower = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
    }
    
    return hasUpper && hasLower;
}

bool UserRegistration::isValidName(const string& name) const {
    if (name.empty() || name.length() > 50) return false;
    
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool UserRegistration::isValidPhone(const string& phone) const {
    if (phone.length() != 10) return false;
    
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool UserRegistration::isUsernameTaken(const string& username) const {
    return users.find(username) != users.end();
}

bool UserRegistration::registerUser() {
    Customer newCustomer;
    string input;
    
    // Get name
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, input);
    
    if (!isValidName(input)) {
        cout << "Invalid name! Name should contain only alphabets and be less than 50 characters." << endl;
        return false;
    }
    newCustomer.name = input;
    
    // Get phone number
    cout << "Enter phone number (10 digits): ";
    getline(cin, input);
    
    if (!isValidPhone(input)) {
        cout << "Invalid phone number! Must be exactly 10 digits." << endl;
        return false;
    }
    newCustomer.phno = input;
    
    // Get bank account number
    cout << "Enter bank account number: ";
    getline(cin, input);
    newCustomer.setBankAccountNo(input);
    
    // Get username
    cout << "Create username: ";
    getline(cin, input);
    
    if (isUsernameTaken(input)) {
        cout << "Username already taken! Please choose a different one." << endl;
        return false;
    }
    newCustomer.username = input;
    
    // Get password
    cout << "Create password (min 8 chars, at least 1 uppercase & 1 lowercase): ";
    getline(cin, input);
    
    if (!isValidPassword(input)) {
        cout << "Invalid password! Must be at least 8 characters with at least 1 uppercase and 1 lowercase letter." << endl;
        return false;
    }
    
    try {
        newCustomer.setPassword(input);  // This now hashes the password using the hash method.
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return false;
    }
    
    // Add user to map
    users[newCustomer.username] = newCustomer;
    cout << "Registration successful!" << endl;
    
    // Save immediately after registration
    saveToFile();
    return true;
}

bool UserRegistration::loginUser() {
    string username, password;
    
    cout << "Enter username: ";
    cin.ignore();
    getline(cin, username);
    
    cout << "Enter password: ";
    getline(cin, password);
    
    auto it = users.find(username);
    if (it == users.end()) {
        cout << "User not found!" << endl;
        return false;
    }
    
    Customer& customer = it->second;
    
    if (customer.isAccountLocked()) {
        cout << "Account locked! Too many failed attempts. Try again after 24 hours." << endl;
        return false;
    }
    
    try {
        if (customer.verifyPassword(password)) {
            customer.resetFailedAttempts();
            cout << "Login successful! Welcome, " << customer.name << "!" << endl;
            saveToFile(); // Save to update failed attempts reset
            return true;
        } else {
            customer.incrementFailedAttempts();
            cout << "Invalid password! Attempts remaining: " << (3 - customer.failedAttempts) << endl;
            
            if (customer.isAccountLocked()) {
                cout << "Account locked for 24 hours due to too many failed attempts!" << endl;
            }
            saveToFile(); // Save to update failed attempts
            return false;
        }
    } catch (const exception& e) {
        cout << "Error during login: " << e.what() << endl;
        return false;
    }
}

void UserRegistration::saveToFile() const {
    ofstream file("users.dat");
    if (!file.is_open()) {
        cout << "Error: Could not save user data to file!" << endl;
        return;
    }
    
    // First line: number of users
    file << users.size() << "\n";
    
    // Save each user
    for (const auto& pair : users) {
        file << pair.second;
    }
    
    file.close();
}

void UserRegistration::loadFromFile() {
    ifstream file("users.dat");
    if (!file.is_open()) {
        cout << "No existing user data found. Starting fresh." << endl;
        return;
    }
    
    try {
        // First line: number of users
        string sizeStr;
        getline(file, sizeStr);
        int userCount = stoi(sizeStr);
        
        // Load each user
        for (int i = 0; i < userCount; i++) {
            Customer cust;
            file >> cust;
            users[cust.username] = cust;
        }
        
        file.close();
        cout << "Loaded " << userCount << " users from file." << endl;
    } catch (const exception& e) {
        cout << "Error loading user data: " << e.what() << endl;
        file.close();
    }
}
