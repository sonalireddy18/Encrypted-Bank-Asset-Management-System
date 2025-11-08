#ifndef SIGNUP_H
#define SIGNUP_H

#include <string>
#include <map>
#include "account.h"

const int MAX_FAILED_ATTEMPTS = 3;
const int LOCK_DURATION = 300; // 5 minutes in seconds

class Customer {
public:
    Customer();

    void setPassword(std::string pass);
    bool checkPassword(const std::string& pass);
    std::string getBankAccountNo() const;

    std::string passwordHash;
    Account* account;

    bool isLocked;
    int failedAttempts;
    time_t lockTime;

    static std::string encryptDecrypt(const std::string& data); // Make public static to avoid inaccessible error

private:
    static std::string hashPassword(const std::string& password);
};

class UserRegistration {
public:
    bool registerUser(const std::string& username, const std::string& password, const std::string& accNo);
    bool authenticateUser(const std::string& username, const std::string& password);
    bool deleteUserByUsername(const std::string& username);

    Customer* getCustomerByUsername(const std::string& username);
    Customer* getCustomerByAccountNo(const std::string& accNo);

    bool saveUsersToFile(const std::string& filename);
    bool loadUsersFromFile(const std::string& filename);

private:
    static std::map<std::string, Customer> users;  // static declaration of users map
};

#endif // SIGNUP_H
