#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Accounts.h"
#include <string>

class Transaction
{
public:
    //Loan handling
    static void loan(Account& account, double loanAmount);

    //Interest handling
    static void interest(Account& account, SavingsAccount& savings, const std::string& tier);

    //Transfers (spending, deposits, etc.)
    //'useCredit' means apply a 2% fee
    static void transfer(Account& from, Account& to, double amount, bool useCredit = false);

    //Bills and Rent
    static void bills(Account& account);

    //Salary deposit
    static void salary(Account& account, double salaryAmount);
};

#endif
