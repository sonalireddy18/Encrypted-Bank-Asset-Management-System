#ifndef SIGNUP_H
#define SIGNUP_H

#include <string>
#include <map>
#include <vector>
#include "account.h"
#include "budget.h"

const int MAX_FAILED_ATTEMPTS = 3;
const int LOCK_DURATION = 300;

class Customer {
public:
    Customer();

    void setPassword(std::string pass);
    bool checkPassword(const std::string& pass);
    std::string getBankAccountNo() const;

    void upgradeAccount();

    std::string passwordHash;
    Account<double>* account;

    bool isLocked;
    int failedAttempts;
    time_t lockTime;

    std::string fullName;
    std::string phoneNumber;

    // ----------------------------------------------------
    // ADD THESE (Budget module data)
    // ----------------------------------------------------
    std::vector<Bill> bills;
    SalaryInfo salary;
    // ----------------------------------------------------

    static std::string encryptDecrypt(const std::string& data);

private:
    static std::string hashPassword(const std::string& password);
};

class UserRegistration {
public:
    bool registerUser(const std::string& username, const std::string& dummy, const std::string& accNo);
    bool authenticateUser(const std::string& username, const std::string& password);
    bool deleteUserByUsername(const std::string& username);

    Customer* getCustomerByUsername(const std::string& username);
    Customer* getCustomerByAccountNo(const std::string& accNo);

    bool saveUsersToFile(const std::string& filename);
    bool loadUsersFromFile(const std::string& filename);

    bool updatePhoneNumber(const std::string& username, const std::string& newPhone);

private:
    static std::map<std::string, Customer> users;
};

#endif
