#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <chrono>
#include <iostream>

class Account {
protected:
    std::string accountNumber;
    double balance;

public:
    Account(const std::string& accNum = "", double bal = 0.0);
    void setBal(double bal);
    double getBal() const;
    std::string getAccNum() const;
    virtual void display() const;
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& accNum = "", double bal = 0.0, double rate = 0.0);
    double getInterestRate() const;
    void setInterestRate(double r);
    void display() const override;
};

class SpendingAmount {
public:
    struct Transaction {
        double amount;
        std::chrono::system_clock::time_point time;
    };

private:
    std::vector<Transaction> transactions;

public:
    void addTransaction(double amount, const std::chrono::system_clock::time_point& timePoint);
    void displaySpentLastDays(int days) const;
    void displayAll() const;
};

class BasicAccount : public Account {
public:
    BasicAccount(const std::string& accNum = "", double bal = 0.0);
    void display() const override;
};

class SilverAccount : public Account {
public:
    SilverAccount(const std::string& accNum = "", double bal = 0.0);
    void display() const override;
};

class GoldAccount : public Account {
public:
    GoldAccount(const std::string& accNum = "", double bal = 0.0);
    void display() const override;
};

#endif // ACCOUNT_H
