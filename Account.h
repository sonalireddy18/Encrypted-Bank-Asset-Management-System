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
    Account(const string& accNum = "", double bal = 0.0) : accountNumber(accNum), balance(bal) {}

    void setBal(double bal) { balance = bal; }
    double getBal() const { return balance; }
    string getAccNum() const { return accountNumber; }

    void display() const;
};

// SAVINGS ACCOUNT CLASS
class SavingsAccount : public Account
{
private:
    double interestRate;

public:
    // Provide all defaults so declaration is valid
    SavingsAccount(const string& accNum = "", double bal = 0.0, double rate = 0.0) : Account(accNum, bal), interestRate(rate) {}

    double getInterestRate() const { return interestRate; }
    void setInterestRate(double r) { interestRate = r; }

    void display() const;
};

// SPENDING AMOUNT CLASS
class SpendingAmount
{
public:
    struct Transaction {
        double amount;
        chrono::system_clock::time_point time;
    };

private:
    vector<Transaction> transactions;

public:
    SpendingAmount() = default;

    const vector<Transaction>& getTransactions() const { return transactions; }

    void addTransaction(double amount, const chrono::system_clock::time_point& timePoint);

    void displaySpentLastDays(int days) const;
    void displaySpentBetween(const chrono::system_clock::time_point& start,
                             const chrono::system_clock::time_point& end) const;
    static chrono::system_clock::time_point makeTimePoint(int year, int month, int day);

    void display() const;
};

// ACCOUNT TIER CLASSES

// BASIC ACCOUNT
class BasicAccount : public Account
{
private:
    SavingsAccount savings;
    SpendingAmount spending;
    double minBalance;

public:
    BasicAccount(const string& accNum = "", double bal = 0.0, double rate = 0.0, double minBal = 0.0);

    double getMinBalance() const { return minBalance; }
    SavingsAccount& getSavingsAccount() { return savings; }
    const SavingsAccount& getSavingsAccount() const { return savings; }
    SpendingAmount& getSpendingAmount() { return spending; }
    const SpendingAmount& getSpendingAmount() const { return spending; }

    void display() const;
};

// SILVER ACCOUNT (kept structure same as basic, different defaults possible)
class SilverAccount : public Account
{
private:
    SavingsAccount savings;
    SpendingAmount spending;
    double minBalance;

public:
    SilverAccount(const string& accNum = "", double bal = 0.0, double rate = 0.0, double minBal = 0.0);

    double getMinBalance() const { return minBalance; }
    const SavingsAccount& getSavingsAccount() const { return savings; }
    const SpendingAmount& getSpendingAmount() const { return spending; }

    void display() const;
};

// GOLD ACCOUNT
class GoldAccount : public Account
{
private:
    SavingsAccount savings;
    SpendingAmount spending;
    double minBalance;

public:
    GoldAccount(const string& accNum = "", double bal = 0.0, double rate = 0.0, double minBal = 0.0);

    double getMinBalance() const { return minBalance; }
    const SavingsAccount& getSavingsAccount() const { return savings; }
    const SpendingAmount& getSpendingAmount() const { return spending; }

    void display() const;
};

#endif
