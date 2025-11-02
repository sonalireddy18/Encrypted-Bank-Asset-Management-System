#include "Accounts.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

// ACCOUNT IMPLEMENTATION
void Account::display() const
{
    cout << "Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n";
}

// SAVINGS ACCOUNT IMPLEMENTATION
void SavingsAccount::display() const
{
    cout << "Savings Account Number: " << accountNumber << "\n"
         << "Balance: " << balance << "\n"
         << "Interest Rate: " << interestRate << "%\n";
}

// SPENDING AMOUNT IMPLEMENTATION
void SpendingAmount::addTransaction(double amount, const chrono::system_clock::time_point& timePoint)
{
    transactions.push_back({amount, timePoint});
}

void SpendingAmount::displaySpentLastDays(int days) const
{
    auto now = chrono::system_clock::now();
    double total = 0;
    for (const auto& t : transactions)
    {
        auto diff = chrono::duration_cast<chrono::hours>(now - t.time).count();
        if (diff <= days * 24)
            total += t.amount;
    }
    cout << "Total spent in last " << days << " day(s): " << total << "\n";
}

void SpendingAmount::displaySpentBetween(const chrono::system_clock::time_point& start, const chrono::system_clock::time_point& end) const
{
    double total = 0;
    for (const auto& t : transactions) {
        if (t.time >= start && t.time <= end)
            total += t.amount;
    }
    cout << "Total spent between specified dates: " << total << "\n";
}

chrono::system_clock::time_point SpendingAmount::makeTimePoint(int year, int month, int day)
{
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    return chrono::system_clock::from_time_t(std::mktime(&tm));
}

void SpendingAmount::display() const {
    cout << "Number of transactions: " << transactions.size() << "\n";
}

// BASIC ACCOUNT IMPLEMENTATION
BasicAccount::BasicAccount(const string& accNum, double bal, double rate, double minBal)
    : Account(accNum, bal), savings(accNum + "-SAV", bal, rate),
      spending(), minBalance(minBal) {}

void BasicAccount::display() const {
    cout << "----- Basic Account -----\n";
    Account::display();
    savings.display();
    spending.display();
    cout << "Minimum Balance: " << minBalance << "\n";
}

// SILVER ACCOUNT IMPLEMENTATION
SilverAccount::SilverAccount(const string& accNum, double bal, double rate, double minBal)
    : Account(accNum, bal), savings(accNum + "-SAV", bal, rate),
      spending(), minBalance(minBal) {}

void SilverAccount::display() const {
    cout << "----- Silver Account -----\n";
    Account::display();
    savings.display();
    spending.display();
    cout << "Minimum Balance: " << minBalance << "\n";
}

// GOLD ACCOUNT IMPLEMENTATION
GoldAccount::GoldAccount(const string& accNum, double bal, double rate, double minBal)
    : Account(accNum, bal), savings(accNum + "-SAV", bal, rate),
      spending(), minBalance(minBal) {}

void GoldAccount::display() const {
    cout << "----- Gold Account -----\n";
    Account::display();
    savings.display();
    spending.display();
    cout << "Minimum Balance: " << minBalance << "\n";
}
