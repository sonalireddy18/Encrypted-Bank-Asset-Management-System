
#include <string>
using namespace std;

class Account
{
private:
    string accountNumber;
    double balance;

public:
    // Constructor
    Account(const string& accNum, double bal)
    {
        accountNumber = accNum;
        balance = bal;
    }

    void setBal(double bal)
    {
        balance = bal;
    }
};


class SavingsAccount: protected Account
{
private:
    double interestRate;

public:
    // Constructor
    SavingsAccount(const string& accNum, double bal, double rate): Account(accNum, bal)
    {
        interestRate = rate;
    }
};

