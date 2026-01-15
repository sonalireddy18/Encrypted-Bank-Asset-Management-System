#include "Budget.h"
#include "Signup.h"
#include "Account.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

//Constructor
BudgetManager::BudgetManager() {}

//Local helper line printer
static void localPrintLine(const string& text) {
    cout << string(text.size(), '-') << "\n";
}

//Member line printer
void BudgetManager::printLine(const string& text) const {
    cout << string(text.size(), '-') << "\n";
}

//Read integer safely
int BudgetManager::readInt() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

//Read double safely
double BudgetManager::readDouble() {
    double d;
    while (!(cin >> d)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return d;
}

//Read and trim whitespace
string BudgetManager::readLineTrim() {
    string s;
    getline(cin, s);
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

//===========================================================
// BILL MANAGEMENT
//===========================================================
void BudgetManager::addBill(Customer* cust) {
    if (!cust) return;
    cout << "\nADD BILL\n";
    printLine("ADD BILL");
    Bill b;

    cout << "Enter bill name: ";
    b.name = readLineTrim();
    while (b.name.empty()) {
        cout << "Bill name cannot be empty. Enter again: ";
        b.name = readLineTrim();
    }

    cout << "Enter bill amount: ";
    b.amount = readDouble();

    cout << "How often is this bill paid?\n";
    cout << "1. daily\n2. weekly\n3. monthly\n4. yearly\n5. custom\n";
    cout << "Choice: ";
    int freq = readInt();
    if (freq == 1) b.frequency = "daily";
    else if (freq == 2) b.frequency = "weekly";
    else if (freq == 3) b.frequency = "monthly";
    else if (freq == 4) b.frequency = "yearly";
    else {
        b.frequency = "custom";
        cout << "Enter custom day interval: ";
        b.customDays = max(1, readInt());
    }

    cust->bills.push_back(b);
    cout << "Bill added successfully.\n";
    printLine("ADD BILL");
}

void BudgetManager::updateBills(Customer* cust) {
    if (!cust) return;
    cout << "\nUPDATE BILLS\n";
    printLine("UPDATE BILLS");

    if (cust->bills.empty()) {
        cout << "No bills to update.\n";
        printLine("UPDATE BILLS");
        return;
    }

    for (size_t i = 0; i < cust->bills.size(); i++) {
        Bill& b = cust->bills[i];
        cout << i+1 << ". " << b.name << " : Rs " << b.amount
             << " (" << b.frequency;
        if (b.frequency == "custom") cout << ", every " << b.customDays << " days";
        cout << ")\n";
    }

    cout << "Select bill to modify (0 to cancel): ";
    int index = readInt();
    if (index <= 0 || index > (int)cust->bills.size()) {
        printLine("UPDATE BILLS");
        return;
    }

    Bill& b = cust->bills[index-1];
    cout << "1. Change name\n2. Change amount\n3. Change frequency\n4. Delete bill\nChoice: ";
    int ch = readInt();

    if (ch == 1) {
        cout << "New name: ";
        b.name = readLineTrim();
    }
    else if (ch == 2) {
        cout << "New amount: ";
        b.amount = readDouble();
    }
    else if (ch == 3) {
        cout << "1=daily 2=weekly 3=monthly 4=yearly 5=custom\nChoice: ";
        int f = readInt();
        if (f == 1) b.frequency = "daily";
        else if (f == 2) b.frequency = "weekly";
        else if (f == 3) b.frequency = "monthly";
        else if (f == 4) b.frequency = "yearly";
        else {
            b.frequency = "custom";
            cout << "Custom days: ";
            b.customDays = max(1, readInt());
        }
    }
    else if (ch == 4) {
        cust->bills.erase(cust->bills.begin() + (index - 1));
        cout << "Bill deleted.\n";
    }
    printLine("UPDATE BILLS");
}

void BudgetManager::viewBills(Customer* cust) {
    if (!cust) return;
    cout << "\nVIEW BILLS\n";
    printLine("VIEW BILLS");

    if (cust->bills.empty()) {
        cout << "No bills.\n";
        printLine("VIEW BILLS");
        return;
    }

    for (size_t i = 0; i < cust->bills.size(); i++) {
        Bill& b = cust->bills[i];
        cout << i+1 << ". " << b.name << " : Rs " << b.amount
             << " (" << b.frequency;
        if (b.frequency == "custom") cout << ", every " << b.customDays << " days";
        cout << ")\n";
    }

    cout << "Edit a bill? (1=yes 0=no): ";
    if (readInt() == 1)
        updateBills(cust);
    printLine("VIEW BILLS");
}

//===========================================================
// SALARY MANAGEMENT
//===========================================================
void BudgetManager::addOrUpdateSalary(Customer* cust) {
    if (!cust) return;
    cout << "\nADD / UPDATE SALARY\n";
    printLine("ADD / UPDATE SALARY");

    cout << "Current salary: Rs " << cust->salary.amount
         << " (" << cust->salary.frequency << ")\n";

    cout << "Enter new salary amount: ";
    double amt = readDouble();
    
    SalaryInfo si;
    si.amount = amt;

    cout << "Frequency?\n";
    cout << "1=daily 2=monthly 3=yearly 4=custom\nChoice: ";
    int f = readInt();
    if (f == 1) si.frequency = "daily";
    else if (f == 2) si.frequency = "monthly";
    else if (f == 3) si.frequency = "yearly";
    else {
        si.frequency = "custom";
        cout << "Enter custom days: ";
        si.customDays = max(1, readInt());
    }

    if (amt > cust->salary.amount)
        cout << "Congratulations! Your salary increased.\n";

    cust->salary = si;
    printLine("ADD / UPDATE SALARY");
}

//===========================================================
// DATE HELPERS
//===========================================================
time_t BudgetManager::readStartDate() {
    return time(nullptr);
}

time_t BudgetManager::readEndDateFromStart(time_t start) {
    cout << "Select period for calculation:\n";
    cout << "1=day 2=week 3=month 4=year 5=custom\nChoice: ";
    int ch = readInt();

    if (ch == 1) return start + 1 * 86400;
    if (ch == 2) return start + 7 * 86400;
    if (ch == 3) return start + 30 * 86400;
    if (ch == 4) return start + 365 * 86400;

    cout << "Custom: 1=end date 2=#days\nChoice: ";
    int mode = readInt();
    if (mode == 1) {
        int d,m,y;
        cout << "Enter DD MM YYYY: ";
        cin >> d >> m >> y;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        tm t = {};
        t.tm_mday = d;
        t.tm_mon = m - 1;
        t.tm_year = y - 1900;
        return mktime(&t);
    } else {
        cout << "Days: ";
        int d = readInt();
        return start + (time_t)d * 86400;
    }
}

int BudgetManager::daysBetween(time_t a, time_t b) {
    double s = difftime(b, a);
    int d = (int)(s / 86400);
    return d < 0 ? 0 : d;
}

int BudgetManager::frequencyToDays(const string& f, int c) const {
    if (f == "daily") return 1;
    if (f == "weekly") return 7;
    if (f == "monthly") return 30;
    if (f == "yearly") return 365;
    if (f == "custom") return max(1, c);
    return 30;
}

//===========================================================
// SPENDING SUMMARY
//===========================================================
void BudgetManager::spendingMenu(Customer* cust, UserRegistration& reg) {
    if (!cust) return;
    cout << "\nSPENDING PERIOD SUMMARY\n";
    printLine("SPENDING PERIOD SUMMARY");

    cout << "1. Current balance\n";
    cout << "2. Calculate for specific period\n";
    cout << "3. Back\nChoice: ";
    int ch = readInt();

    if (ch == 1) {
        cout << "Current Amount: Rs " << (cust->account ? cust->account->getBal() : 0.0) << "\n";
        return;
    }
    if (ch == 3) return;

    time_t start = readStartDate();
    time_t endt = readEndDateFromStart(start);
    int totalDays = daysBetween(start, endt);
    if (totalDays < 1) totalDays = 1;

    // Fixed Custom Days Logic for Salary
    int sd = frequencyToDays(cust->salary.frequency, cust->salary.customDays);
    int salaryOccurrences = totalDays / max(1, sd);
    if (salaryOccurrences == 0 && totalDays > 0) salaryOccurrences = 1; 
    double salaryTotal = cust->salary.amount * salaryOccurrences;

    // Fixed Custom Days Logic for Bills
    double billsTotal = 0.0;
    vector<pair<string,double>> billLines;
    for (auto& b : cust->bills) {
        int bd = frequencyToDays(b.frequency, b.customDays);
        int billOccurrences = totalDays / max(1, bd);
        if (billOccurrences < 0) billOccurrences = 0;
        double total = b.amount * billOccurrences;
        billsTotal += total;
        billLines.push_back({b.name, total});
    }

    double currentBal = cust->account ? cust->account->getBal() : 0.0;
    cout << "\n--- SUMMARY FOR " << totalDays << " DAYS ---\n";
    cout << "Initial Balance: Rs " << currentBal << "\n";
    for (auto& p : billLines)
        cout << p.first << ": - Rs " << p.second << "\n";
    cout << "Projected Salary: + Rs " << salaryTotal << "\n";
    cout << string(40, '-') << "\n";

    double totalSpendable = currentBal + salaryTotal - billsTotal;
    cout << "TOTAL PROJECTED SPENDABLE: Rs " << totalSpendable << "\n";
    cout << "REQUIRED SAVINGS FOR BILLS: Rs " << billsTotal << "\n";

    cout << "Transfer savings to savings account? (y/n): ";
    char r; cin >> r;
    if (r == 'y' || r == 'Y') {
        cout << "Feature Verification: Use the main menu 'Transfer' to move real funds.\n";
    }
    printLine("SPENDING PERIOD SUMMARY");
}

void BudgetManager::showBudgetMenu(Customer* cust, UserRegistration& reg) {
    if (!cust) return;
    while (true) {
        cout << "\nBUDGET DASHBOARD\n";
        printLine("BUDGET DASHBOARD");
        cout << "1. Bills\n2. Salary\n3. Spending Summary\n4. Back\nChoice: ";
        int ch = readInt();

        if (ch == 1) {
            cout << "1. Add 2. Update 3. View 4. Back\nChoice: ";
            int b = readInt();
            if (b == 1) addBill(cust);
            else if (b == 2) updateBills(cust);
            else if (b == 3) viewBills(cust);
        }
        else if (ch == 2) addOrUpdateSalary(cust);
        else if (ch == 3) spendingMenu(cust, reg);
        else break;
    }
}
