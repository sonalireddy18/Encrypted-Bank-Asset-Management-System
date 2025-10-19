#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>
#include <chrono>
#include <vector>
using namespace std;


// BASE ACCOUNT CLASS
class Account
{
protected:
    string accountNumber;
    double balance;

public:
    Account(const string& accNum = "", double bal = 0): accountNumber(accNum), balance(bal) {}

    void setBal(double bal) { balance = bal; }
    double getBal() const { return balance; }
    string getAccNum() const { return accountNumber; }

    void display() const; 
};


// SAVINGS ACCOUNT CLASS
class SavingsAccount : protected Account
{
private:
    double interestRate;

public:
    SavingsAccount(const string& accNum = "", double bal, double rate): Account(accNum, bal), interestRate(rate) {}

    double getInterestRate() const { return interestRate; }

    void display() const; 
};


// SPENDING AMOUNT CLASS
class SpendingAmount
{
private:
    struct Transaction {
        double amount;
        std::chrono::system_clock::time_point time;
    };

    vector<Transaction> transactions;

public:
    SpendingAmount() = default;

    const vector<Transaction>& getTransactions() const { return transactions; }

    void displaySpentLastDays(int days) const;
    void displaySpentBetween(const std::chrono::system_clock::time_point& start,
                             const std::chrono::system_clock::time_point& end) const;
    static std::chrono::system_clock::time_point makeTimePoint(int year, int month, int day);

    void display() const;
};


// ACCOUNT TIER CLASSES

// BASIC ACCOUNT
class BasicAccount : protected Account
{
private:
    SavingsAccount savings;
    SpendingAmount spending;
    double minBalance;

public:
    BasicAccount(const string& accNum, double bal, double rate, double minBal);

    double getMinBalance() const { return minBalance; }
    const SavingsAccount& getSavingsAccount() const { return savings; }
    const SpendingAmount& getSpendingAmount() const { return spending; }

    void display() const;
};

// SILVER ACCOUNT
class SilverAccount : protected Account
{
private:
    SavingsAccount savings;
    SpendingAmount spending;
    double minBalance;

public:
    SilverAccount(const string& accNum, double bal, double rate, double minBal);

    double getMinBalance() const { return minBalance; }
    const SavingsAccount& getSavingsAccount() const { return savings; }
    const SpendingAmount& getSpendingAmount() const { return spending; }

    void display() const;
};

// GOLD ACCOUNT
class GoldAccount : protected Account
{
private:
    SavingsAccount savings;
    SpendingAmount spending;
    double minBalance;

public:
    GoldAccount(const string& accNum, double bal, double rate, double minBal);

    double getMinBalance() const { return minBalance; }
    const SavingsAccount& getSavingsAccount() const { return savings; }
    const SpendingAmount& getSpendingAmount() const { return spending; }

    void display() const;
};

#endif
