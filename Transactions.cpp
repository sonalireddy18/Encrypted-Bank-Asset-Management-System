#include "transaction.h"
#include <iostream>
#include "block.h"
#include "signup.h"
#include "account.h"S

using namespace std;

Blockchain Transaction::blockchain;

void Transaction::transferByAccountNoUserRegistration(UserRegistration& userReg, const std::string& fromAccNo, const std::string& toAccNo, double amount) {
    Customer* fromCust = userReg.getCustomerByAccountNo(fromAccNo);
    Customer* toCust = userReg.getCustomerByAccountNo(toAccNo);

    if (fromCust && fromCust->account && toCust && toCust->account) {
        if (fromCust->account->getBal() >= amount) {
            fromCust->account->setBal(fromCust->account->getBal() - amount);
            toCust->account->setBal(toCust->account->getBal() + amount);
            cout << "Transfer successful.\n";

            // Add transfer to blockchain
            blockchain.addTransaction(fromAccNo, toAccNo, amount);
        } else {
            cout << "Insufficient funds for transfer.\n";
        }
    } else {
        cout << "One or both account numbers are invalid.\n";
    }
}

void Transaction::withdrawUserRegistration(UserRegistration& userReg, const std::string& accNo, double amount) {
    Customer* cust = userReg.getCustomerByAccountNo(accNo);
    if (cust && cust->account) {
        if (cust->account->getBal() >= amount) {
            cust->account->setBal(cust->account->getBal() - amount);
            cout << "Withdrawal successful.\n";
            // Add withdrawal as a transaction to blockchain; toAccNo empty for withdrawal
            blockchain.addTransaction(accNo, "", amount);
        } else {
            cout << "Insufficient balance to withdraw.\n";
        }
    } else {
        cout << "Invalid account number.\n";
    }
}

void Transaction::depositUserRegistration(UserRegistration& userReg, const std::string& accNo, double amount) {
    Customer* cust = userReg.getCustomerByAccountNo(accNo);
    if (cust && cust->account) {
        cust->account->setBal(cust->account->getBal() + amount);
        cout << "Deposit successful.\n";
        // Add deposit as a transaction to blockchain; fromAccNo empty for deposit
        blockchain.addTransaction("", accNo, amount);
    } else {
        cout << "Invalid account number.\n";
    }
}

void Transaction::printTransactionHistory() {
    blockchain.printChain();
}
