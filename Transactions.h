#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include "signup.h"
#include "block.h"

class Transaction {
public:
    static Blockchain blockchain;

    static void transferByAccountNoUserRegistration(UserRegistration& userReg, const std::string& fromAccNo, const std::string& toAccNo, double amount);
    static void withdrawUserRegistration(UserRegistration& userReg, const std::string& accNo, double amount);
    static void depositUserRegistration(UserRegistration& userReg, const std::string& accNo, double amount);
    static void printTransactionHistory();


    static void saveBlockchainToFile(const std::string& filename);
    static void loadBlockchainFromFile(const std::string& filename);
};

#endif // TRANSACTION_H
