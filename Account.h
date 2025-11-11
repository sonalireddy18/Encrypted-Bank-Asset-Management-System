#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>
#include <iostream>
using namespace std;

//Account tier constants
constexpr int MIN_BASIC  = 0;
constexpr int MIN_SILVER = 10000;
constexpr int MIN_GOLD   = 100000;

constexpr double INT_BASIC  = 2.0;
constexpr double INT_SILVER = 5.0;
constexpr double INT_GOLD   = 10.0;

//Function to print account tier information
inline void printTierInfo() {
    cout << "\n--- ACCOUNT TIERS ---\n";
    cout << "NORMAL ACCOUNTS:\n";
    cout << " Basic  : MinBal " << MIN_BASIC  << "\n";
    cout << " Silver : MinBal " << MIN_SILVER << "\n";
    cout << " Gold   : MinBal " << MIN_GOLD   << "\n\n";
    cout << "SAVINGS ACCOUNTS:\n";
    cout << " Basic  : MinBal " << MIN_BASIC  << ", Interest " << INT_BASIC  << "%\n";
    cout << " Silver : MinBal " << MIN_SILVER << ", Interest " << INT_SILVER << "%\n";
    cout << " Gold   : MinBal " << MIN_GOLD   << ", Interest " << INT_GOLD   << "%\n";
    cout << "--------------------\n\n";
}


// Base Account Template
template <typename T>
class Account {
protected:
    string accountNumber;
    //Using template for balance to allow flexibility
    T balance;

public:
    Account(const string& accNum = "", T bal = 0.0);
    virtual ~Account() {}
    void setBal(T bal);
    T getBal() const;
    string getAccNum() const;
    virtual void display() const;
};


// Derived Normal Accounts
class BasicAccount : public Account<double> {
public:
    static constexpr int MinBal = MIN_BASIC;
    BasicAccount(const string&, double);
    void display() const override;
};

class SilverAccount : public Account<double> {
public:
    static constexpr int MinBal = MIN_SILVER;
    SilverAccount(const string&, double);
    void display() const override;
};

class GoldAccount : public Account<double> {
public:
    static constexpr int MinBal = MIN_GOLD;
    GoldAccount(const string&, double);
    void display() const override;
};


// Derived Savings Accounts
class SavingsAccount : public Account<double> {
protected:
    double interestRate;
public:
    SavingsAccount(const string&, double, double);
    double getInterestRate() const;
    void applyInterest();
    void display() const override;
};

class SavingsBasic : public SavingsAccount {
public:
    static constexpr int MinBal = MIN_BASIC;
    SavingsBasic(const string&, double);
    void display() const override;
};

class SavingsSilver : public SavingsAccount {
public:
    static constexpr int MinBal = MIN_SILVER;
    SavingsSilver(const string&, double);
    void display() const override;
};

class SavingsGold : public SavingsAccount {
public:
    static constexpr int MinBal = MIN_GOLD;
    SavingsGold(const string&, double);
    void display() const override;
};

// Explicit template instantiation
extern template class Account<double>;

#endif
