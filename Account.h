#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>
#include <iostream>

constexpr int MIN_BASIC  = 0;
constexpr int MIN_SILVER = 10000;
constexpr int MIN_GOLD   = 100000;

constexpr double INT_BASIC  = 2.0;
constexpr double INT_SILVER = 5.0;
constexpr double INT_GOLD   = 10.0;

inline void printTierInfo() {
    std::cout << "\n--- ACCOUNT TIERS ---\n";
    std::cout << "NORMAL ACCOUNTS:\n";
    std::cout << " Basic  : MinBal " << MIN_BASIC  << "\n";
    std::cout << " Silver : MinBal " << MIN_SILVER << "\n";
    std::cout << " Gold   : MinBal " << MIN_GOLD   << "\n\n";
    std::cout << "SAVINGS ACCOUNTS:\n";
    std::cout << " Basic  : MinBal " << MIN_BASIC  << ", Interest " << INT_BASIC  << "%\n";
    std::cout << " Silver : MinBal " << MIN_SILVER << ", Interest " << INT_SILVER << "%\n";
    std::cout << " Gold   : MinBal " << MIN_GOLD   << ", Interest " << INT_GOLD   << "%\n";
    std::cout << "--------------------\n\n";
}

// BASE ACCOUNT
class Account {
protected:
    std::string accountNumber;
    double balance;

public:
    Account(const std::string& accNum = "", double bal = 0.0);
    virtual ~Account() {}
    void setBal(double bal);
    double getBal() const;
    std::string getAccNum() const;
    virtual void display() const;
};

// NORMAL ACCOUNTS
class BasicAccount : public Account {
public:
    static constexpr int MinBal = MIN_BASIC;
    BasicAccount(const std::string&, double);
    void display() const override;
};

class SilverAccount : public Account {
public:
    static constexpr int MinBal = MIN_SILVER;
    SilverAccount(const std::string&, double);
    void display() const override;
};

class GoldAccount : public Account {
public:
    static constexpr int MinBal = MIN_GOLD;
    GoldAccount(const std::string&, double);
    void display() const override;
};

// SAVINGS ACCOUNTS
class SavingsAccount : public Account {
protected:
    double interestRate;
public:
    SavingsAccount(const std::string&, double, double);
    double getInterestRate() const;
    void applyInterest();
    void display() const override;
};

class SavingsBasic : public SavingsAccount {
public:
    static constexpr int MinBal = MIN_BASIC;
    SavingsBasic(const std::string&, double);
    void display() const override;
};

class SavingsSilver : public SavingsAccount {
public:
    static constexpr int MinBal = MIN_SILVER;
    SavingsSilver(const std::string&, double);
    void display() const override;
};

class SavingsGold : public SavingsAccount {
public:
    static constexpr int MinBal = MIN_GOLD;
    SavingsGold(const std::string&, double);
    void display() const override;
};

#endif
