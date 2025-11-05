#include "signup.h"
#include "transaction.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// Customer default constructor
Customer::Customer() : isLocked(false), failedAttempts(0), lockTime(0), account(nullptr) {}

// Simple DJB2 hash function for password hashing
string Customer::hashPassword(const string& password) {
    unsigned int hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) ^ c;
    }
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

// Symmetric XOR encryption/decryption for storing password (basic)
string Customer::encryptDecrypt(const string& data) {
    char key = 'K'; 
    string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key;
    }
    return result;
}

void Customer::setPassword(string pass) {
    passwordHash = hashPassword(pass);
}

bool Customer::checkPassword(const string& pass) {
    return passwordHash == hashPassword(pass);
}

string Customer::getBankAccountNo() const {
    if (account) return account->getAccNum();
    return "";
}

// -------- UserRegistration class implementation --------

Customer* UserRegistration::getCustomerByUsername(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) return &(it->second);
    return nullptr;
}

Customer* UserRegistration::getCustomerByAccountNo(const string& accNo) {
    for (auto& pair : users) {
        if (pair.second.getBankAccountNo() == accNo) return &(pair.second);
    }
    return nullptr;
}

bool UserRegistration::registerUser(const string& username, const string& password, const string& accNo) {
    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
        return false;
    }
    Customer newCustomer;
    newCustomer.setPassword(password);
    newCustomer.account = new BasicAccount(accNo, 0.0);
    users[username] = newCustomer;
    cout << "User registered successfully.\n";
    return true;
}

bool UserRegistration::authenticateUser(const string& username, const string& password) {
    Customer* cust = getCustomerByUsername(username);
    if (!cust) {
        cout << "User does not exist.\n";
        return false;
    }
    if (cust->isLocked) {
        time_t currentTime = time(nullptr);
        if (difftime(currentTime, cust->lockTime) >= LOCK_DURATION) {
            cust->isLocked = false;
            cust->failedAttempts = 0;
        } else {
            cout << "Account is locked. Try again later.\n";
            return false;
        }
    }
    if (cust->checkPassword(password)) {
        cust->failedAttempts = 0;
        return true;
    } else {
        cust->failedAttempts++;
        cout << "Incorrect password.\n";
        if (cust->failedAttempts >= MAX_FAILED_ATTEMPTS) {
            cust->isLocked = true;
            cust->lockTime = time(nullptr);
            cout << "Account locked due to multiple failed attempts.\n";
        }
        return false;
    }
}

bool UserRegistration::deleteUserByUsername(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        delete it->second.account;
        users.erase(it);
        cout << "User deleted successfully.\n";
        return true;
    }
    cout << "User not found.\n";
    return false;
}
