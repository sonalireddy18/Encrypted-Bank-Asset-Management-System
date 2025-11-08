#include "Accounts.h"
#include <iostream>
using namespace std;

Account::Account(const string& accNum, double bal) : accountNumber(accNum), balance(bal) {}
void Account::setBal(double bal) { balance = bal; }
double Account::getBal() const { return balance; }
string Account::getAccNum() const { return accountNumber; }
void Account::display() const {
    cout << "Account Number: " << accountNumber << ", Balance: " << balance << endl;
}

// NORMAL
BasicAccount::BasicAccount(const string& accNum, double bal) : Account(accNum, bal) {}
void BasicAccount::display() const {
    cout << "Normal Basic (minBal " << MinBal << ") - " << accountNumber << ", Bal: " << balance << endl;
}
SilverAccount::SilverAccount(const string& accNum, double bal) : Account(accNum, bal) {}
void SilverAccount::display() const {
    cout << "Normal Silver (minBal " << MinBal << ") - " << accountNumber << ", Bal: " << balance << endl;
}
GoldAccount::GoldAccount(const string& accNum, double bal) : Account(accNum, bal) {}
void GoldAccount::display() const {
    cout << "Normal Gold (minBal " << MinBal << ") - " << accountNumber << ", Bal: " << balance << endl;
}

// SAVINGS
SavingsAccount::SavingsAccount(const string& accNum, double bal, double rate) : Account(accNum, bal), interestRate(rate) {}
double SavingsAccount::getInterestRate() const { return interestRate; }
void SavingsAccount::applyInterest() { balance += (balance * interestRate / 100.0); }
void SavingsAccount::display() const {
    cout << "Savings (rate " << interestRate << "%) - " << accountNumber << ", Bal: " << balance << endl;
}

SavingsBasic::SavingsBasic(const string& accNum, double bal) : SavingsAccount(accNum, bal, INT_BASIC) {}
void SavingsBasic::display() const {
    cout << "Savings Basic (" << MinBal << ", " << INT_BASIC << "%) - " << accountNumber << ", Bal: " << balance << endl;
}
SavingsSilver::SavingsSilver(const string& accNum, double bal) : SavingsAccount(accNum, bal, INT_SILVER) {}
void SavingsSilver::display() const {
    cout << "Savings Silver (" << MinBal << ", " << INT_SILVER << "%) - " << accountNumber << ", Bal: " << balance << endl;
}
SavingsGold::SavingsGold(const string& accNum, double bal) : SavingsAccount(accNum, bal, INT_GOLD) {}
void SavingsGold::display() const {
    cout << "Savings Gold (" << MinBal << ", " << INT_GOLD << "%) - " << accountNumber << ", Bal: " << balance << endl;
}
