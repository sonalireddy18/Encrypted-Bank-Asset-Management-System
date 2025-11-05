#include "Accounts.h"
#include <iostream>

using namespace std;

// Account methods
Account::Account(const std::string& accNum, double bal) : accountNumber(accNum), balance(bal) {}

void Account::setBal(double bal) {
    balance = bal;
}

double Account::getBal() const {
    return balance;
}

void Account::printAccountBalance() const {
    cout << "Account Number: " << accountNumber << ", Balance: " << balance << "\n";
}

std::string Account::getAccNum() const {
    return accountNumber;
}

void Account::display() const {
    cout << "Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n";
}

// SavingsAccount methods
SavingsAccount::SavingsAccount(const std::string& accNum, double bal, double rate) : Account(accNum, bal), interestRate(rate) {}

double SavingsAccount::getInterestRate() const {
    return interestRate;
}

void SavingsAccount::setInterestRate(double r) {
    interestRate = r;
}

void SavingsAccount::display() const {
    cout << "Savings Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n"
         << "Interest Rate: " << interestRate << "%\n";
}

// SpendingAmount methods
void SpendingAmount::addTransaction(double amount, const chrono::system_clock::time_point& timePoint) {
    transactions.push_back({amount, timePoint});
}

void SpendingAmount::displaySpentLastDays(int days) const {
    auto now = chrono::system_clock::now();
    double total = 0;
    for (const auto& t : transactions) {
        auto diff = chrono::duration_cast<chrono::hours>(now - t.time).count();
        if (diff <= days * 24) {
            total += t.amount;
        }
    }
    cout << "Total spent in last " << days << " day(s): " << total << "\n";
}

void SpendingAmount::displayAll() const {
    cout << "All Transactions:\n";
    for (const auto& t : transactions) {
        time_t tt = chrono::system_clock::to_time_t(t.time);
        cout << "Amount: " << t.amount << ", Time: " << ctime(&tt);
    }
}

// BasicAccount methods
BasicAccount::BasicAccount(const std::string& accNum, double bal) : Account(accNum, bal) {}

void BasicAccount::display() const {
    cout << "Basic Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n";
}

// SilverAccount methods
SilverAccount::SilverAccount(const std::string& accNum, double bal) : Account(accNum, bal) {}

void SilverAccount::display() const {
    cout << "Silver Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n";
}

// GoldAccount methods
GoldAccount::GoldAccount(const std::string& accNum, double bal) : Account(accNum, bal) {}

void GoldAccount::display() const {
    cout << "Gold Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n";
}
