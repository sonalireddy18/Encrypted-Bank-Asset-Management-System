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

    // Shows only current user's transactions
    static void printTransactionHistory(const std::string& accNo);

    // show entire blockchain ledger
    static void printFullBlockchain();

    static void saveBlockchainToFile(const std::string& filename);
    static void loadBlockchainFromFile(const std::string& filename);
};

#endif // TRANSACTION_H
