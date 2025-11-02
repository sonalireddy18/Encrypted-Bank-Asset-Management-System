#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// LOAN HANDLER
void Transaction::loan(Account& account, double loanAmount)
{
    cout << "Processing loan of $" << fixed << setprecision(2) << loanAmount << "...\n";
    account.setBal(account.getBal() + loanAmount);
    cout << "Loan credited. New balance: $" << account.getBal() << "\n";
}

// INTEREST HANDLER
void Transaction::interest(Account& account, SavingsAccount& savings, const std::string& tier)
{
    double withdrawInterestRate = 0.05; // Fixed loan withdrawal interest
    double depositInterestRate = 0.0;

    if (tier == "Basic") depositInterestRate = 0.03;
    else if (tier == "Silver") depositInterestRate = 0.05;
    else if (tier == "Gold") depositInterestRate = 0.10;

    double withdrawalInterest = account.getBal() * withdrawInterestRate;
    double depositInterest = savings.getBal() * depositInterestRate;

    account.setBal(account.getBal() - withdrawalInterest);
    savings.setBal(savings.getBal() + depositInterest);

    cout << fixed << setprecision(2);
    cout << "Applied interest rates based on tier (" << tier << ")\n";
    cout << "Loan withdrawal interest deducted: $" << withdrawalInterest << "\n";
    cout << "Savings interest added: $" << depositInterest << "\n";
}

// TRANSFER HANDLER
void Transaction::transfer(Account& from, Account& to, double amount, bool useCredit)
{
    double fee = useCredit ? amount * 0.02 : 0.0;
    double total = amount + fee;

    cout << fixed << setprecision(2);

    if (from.getBal() >= total)
    {
        from.setBal(from.getBal() - total);
        to.setBal(to.getBal() + amount);

        cout << "Transferred $" << amount << " from " << from.getAccNum()
             << " to " << to.getAccNum() << ".\n";
        if (useCredit) cout << "Fee applied: $" << fee << "\n";
        cout << "Sender new balance: $" << from.getBal() << "\n";
        cout << "Receiver new balance: $" << to.getBal() << "\n";
    }
    else {
        cout << "Insufficient funds for transfer.\n";
    }
}

// BILLS HANDLER
void Transaction::bills(Account& account)
{
    int numBills;
    cout << "Enter number of bills/rent payments: ";
    cin >> numBills;

    double total = 0.0;
    for (int i = 0; i < numBills; ++i)
    {
        double amount;
        cout << "Enter amount for bill #" << i + 1 << ": $";
        cin >> amount;
        total += amount;
    }

    cout << fixed << setprecision(2);
    if (account.getBal() >= total)
    {
        account.setBal(account.getBal() - total);
        cout << "Total of $" << total << " paid successfully.\n";
        cout << "Remaining balance: $" << account.getBal() << "\n";
    }
    else {
        cout << "Insufficient funds to cover $" << total << " in bills.\n";
    }
}

// SALARY HANDLER
void Transaction::salary(Account& account, double salaryAmount)
{
    cout << fixed << setprecision(2);
    cout << "Depositing salary of $" << salaryAmount << "...\n";
    account.setBal(account.getBal() + salaryAmount);
    cout << "Salary deposited. New balance: $" << account.getBal() << "\n";
}
