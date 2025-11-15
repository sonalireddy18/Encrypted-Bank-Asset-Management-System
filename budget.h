#ifndef BUDGET_H
#define BUDGET_H

#include <string>
#include <vector>
#include <ctime>

// Forward declarations to avoid circular include with signup.h
struct Customer;
class UserRegistration;

// Budget is a DISPLAY MODULE only. It does NOT modify account balances.

struct Bill {
    std::string name;
    double amount;
    // frequency type: "daily", "weekly", "monthly", "yearly", "custom"
    std::string frequency;
    // for custom frequency: number of days between bills (1..n)
    int customDays;

    Bill() : amount(0.0), frequency("monthly"), customDays(0) {}
};

struct SalaryInfo {
    double amount;
    // frequency type: "daily", "monthly", "yearly", "custom"
    std::string frequency;
    int customDays;

    SalaryInfo() : amount(0.0), frequency("monthly"), customDays(0) {}
};

class BudgetManager {
public:
    BudgetManager();

    // Bills
    void addBill(Customer* cust);
    void updateBills(Customer* cust);
    void viewBills(Customer* cust);

    // Salary
    void addOrUpdateSalary(Customer* cust);

    // Spending display
    // Uses logged-in customer pointer to fetch CURRENT BALANCE for display only.
    void spendingMenu(Customer* cust, UserRegistration& reg);

    // Main budget menu for logged in user
    void showBudgetMenu(Customer* cust, UserRegistration& reg);

private:
    // helpers
    int readInt();
    double readDouble();
    std::string readLineTrim();
    std::time_t readStartDate(); // returns now (start)
    std::time_t readEndDateFromStart(std::time_t start);
    int daysBetween(std::time_t from, std::time_t to);
    int frequencyToDays(const std::string& freq, int customDays) const;
    void printLine(const std::string& text) const;
};

#endif // BUDGET_H
