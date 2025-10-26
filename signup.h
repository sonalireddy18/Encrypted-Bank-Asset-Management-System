#ifndef SIGNUP_H
#define SIGNUP_H

#include <string>
#include <unordered_map>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

class Customer {
private:
    string passwordHash;  
    string bankAccountNo;
    bool isLocked;
    time_t lockTime;
public:
    int failedAttempts;
    string name;
    string username;
    string phno;
    Customer();
    void setPassword(string pass);
    bool verifyPassword(string pass) const;
    bool isAccountLocked() const;
    void incrementFailedAttempts();
    void resetFailedAttempts();
    void setBankAccountNo(string accNo);
    string getBankAccountNo() const;

    // Encryption + Hashing
    static string hashPassword(const string& password);
    static string encryptDecrypt(const string& data);

    // File operations
    friend ofstream& operator<<(ofstream& ofs, const Customer& cust);
    friend ifstream& operator>>(ifstream& ifs, Customer& cust);
};
class UserRegistration {
private:
    unordered_map<string, Customer> users;
    bool isValidPassword(const string& password) const;
    bool isValidName(const string& name) const;
    bool isValidPhone(const string& phone) const;
    bool isUsernameTaken(const string& username) const;
public:
    UserRegistration();
    ~UserRegistration();

    bool registerUser();
    bool loginUser();
    void saveToFile() const;
    void loadFromFile();
};

#endif
