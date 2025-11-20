#include "Budget.h"
#include "signup.h"
#include "account.h"
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
    size_t end   = s.find_last_not_of(" \t\r\n");

    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}


//===========================================================
// BILL MANAGEMENT
//===========================================================

//Add a bill to customer
void BudgetManager::addBill(Customer* cust) {
    if (!cust) return;

    cout << "\nADD BILL\n";
    printLine("ADD BILL");

    Bill b;

    //Bill name
    cout << "Enter bill name: ";
    b.name = readLineTrim();
    while (b.name.empty()) {
        cout << "Bill name cannot be empty. Enter again: ";
        b.name = readLineTrim();
    }

    //Bill amount
    cout << "Enter bill amount: ";
    b.amount = readDouble();

    //Bill frequency
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

    //Store bill
    cust->bills.push_back(b);

    cout << "Bill added successfully.\n";
    printLine("ADD BILL");
}

//Update bills list
void BudgetManager::updateBills(Customer* cust) {
    if (!cust) return;

    cout << "\nUPDATE BILLS\n";
    printLine("UPDATE BILLS");

    //Check if empty
    if (cust->bills.empty()) {
        cout << "No bills to update.\n";
        printLine("UPDATE BILLS");
        return;
    }

    //List bills
    for (size_t i = 0; i < cust->bills.size(); i++) {
        Bill& b = cust->bills[i];
        cout << i+1 << ". " << b.name << " : Rs " << b.amount
             << " (" << b.frequency;
        if (b.frequency == "custom") cout << ", every " << b.customDays << " days";
        cout << ")\n";
    }

    //Select bill
    cout << "Select bill to modify (0 to cancel): ";
    int index = readInt();
    if (index <= 0 || index > (int)cust->bills.size()) {
        printLine("UPDATE BILLS");
        return;
    }
    Bill& b = cust->bills[index-1];

    //Choose action
    cout << "1. Change name\n";
    cout << "2. Change amount\n";
    cout << "3. Change frequency\n";
    cout << "4. Delete bill\n";
    cout << "Choice: ";
    int ch = readInt();

    //Modify bill
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
            b.customDays = readInt();
        }
    }
    else if (ch == 4) {
        cust->bills.erase(cust->bills.begin() + (index - 1));
        cout << "Bill deleted.\n";
    }

    printLine("UPDATE BILLS");
}

//View bills and optionally edit
void BudgetManager::viewBills(Customer* cust) {
    if (!cust) return;

    cout << "\nVIEW BILLS\n";
    printLine("VIEW BILLS");

    //If none
    if (cust->bills.empty()) {
        cout << "No bills.\n";
        printLine("VIEW BILLS");
        return;
    }

    //Display list
    for (size_t i = 0; i < cust->bills.size(); i++) {
        Bill& b = cust->bills[i];
        cout << i+1 << ". " << b.name << " : Rs " << b.amount
             << " (" << b.frequency;
        if (b.frequency == "custom") cout << ", every " << b.customDays << " days";
        cout << ")\n";
    }

    //Edit option
    cout << "Edit a bill? (1=yes 0=no): ";
    if (readInt() == 1)
        updateBills(cust);

    printLine("VIEW BILLS");
}


//===========================================================
// SALARY MANAGEMENT
//===========================================================

//Add or update customer salary
void BudgetManager::addOrUpdateSalary(Customer* cust) {
    if (!cust) return;

    cout << "\nADD / UPDATE SALARY\n";
    printLine("ADD / UPDATE SALARY");

    //Display current salary
    cout << "Current salary: Rs " << cust->salary.amount
         << " (" << cust->salary.frequency << ")\n";

    //Enter amount
    cout << "Enter new salary amount: ";
    double amt = readDouble();

    SalaryInfo si;
    si.amount = amt;

    //Frequency choice
    cout << "Frequency?\n";
    cout << "1=daily 2=monthly 3=yearly 4=custom\nChoice: ";
    int f = readInt();

    if (f == 1) si.frequency = "daily";
    else if (f == 2) si.frequency = "monthly";
    else if (f == 3) si.frequency = "yearly";
    else {
        si.frequency = "custom";
        cout << "Enter custom days: ";
        si.customDays = readInt();
    }

    //Raise message
    if (amt > cust->salary.amount)
        cout << "Congratulations! Your salary increased.\n";

    //Update
    cust->salary = si;

    printLine("ADD / UPDATE SALARY");
}


//===========================================================
// DATE HELPERS
//===========================================================

//Return current time as start date
time_t BudgetManager::readStartDate() {
    return time(nullptr);
}

//Calculate end date
time_t BudgetManager::readEndDateFromStart(time_t start) {
    cout << "1=day 2=week 3=month 4=year 5=custom\nChoice: ";
    int ch = readInt();

    //Standard offsets
    if (ch == 1) return start + 1 * 86400;
    if (ch == 2) return start + 7 * 86400;
    if (ch == 3) return start + 30 * 86400;
    if (ch == 4) return start + 365 * 86400;

    //Custom mode
    cout << "Custom: 1=end date 2=#days\nChoice: ";
    int mode = readInt();

    if (mode == 1) {
        int d,m,y;
        cout << "Enter DD MM YYYY: ";
        cin >> d >> m >> y;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        tm t = {};
        t.tm_mday = d;
        t.tm_mon  = m - 1;
        t.tm_year = y - 1900;

        return mktime(&t);
    }
    else {
        cout << "Days: ";
        int d = readInt();
        return start + d * 86400;
    }
}

//Return days difference
int BudgetManager::daysBetween(time_t a, time_t b) {
    double s = difftime(b, a);
    int d = (int)(s / 86400);
    return d < 0 ? 0 : d;
}

//Convert frequency to days
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

//Spending menu handler
void BudgetManager::spendingMenu(Customer* cust, UserRegistration& reg) {
    if (!cust) return;

    cout << "\nSPENDING\n";
    printLine("SPENDING");

    //Menu
    cout << "1. Current balance\n";
    cout << "2. Spending period summary\n";
    cout << "3. Back\n";
    cout << "Choice: ";
    int ch = readInt();

    //Balance only
    if (ch == 1) {
        cout << "Current Amount: Rs "
             << (cust->account ? cust->account->getBal() : 0.0)
             << "\n";
        printLine("SPENDING");
        return;
    }

    //Go back
    if (ch == 3) {
        printLine("SPENDING");
        return;
    }

    //Start date
    time_t start = readStartDate();
    time_t endt  = readEndDateFromStart(start);

    int days = daysBetween(start, endt);
    if (days < 1) days = 1;

    //Salary income total
    int sd = frequencyToDays(cust->salary.frequency, cust->salary.customDays);
    int occ = days / max(1, sd);
    if (occ == 0) occ = 1;
    double salaryTotal = cust->salary.amount * occ;

    //Bills total
    double billsTotal = 0.0;
    vector<pair<string,double>> billLines;

    for (auto& b : cust->bills) {
        int bd = frequencyToDays(b.frequency, b.customDays);
        int bo = days / max(1, bd);
        if (bo < 0) bo = 0;

        double total = b.amount * bo;
        billsTotal += total;

        billLines.push_back({b.name, total});
    }

    //Current balance
    double currentBal = cust->account ? cust->account->getBal() : 0.0;

    cout << "Current Amount: Rs " << currentBal << "\n";

    //Print bills
    for (auto& p : billLines)
        cout << p.first << ": - Rs " << p.second << "\n";

    //Print salary
    cout << "Salary: + Rs " << salaryTotal << "\n";

    cout << string(60, '-') << "\n";

    //Totals
    double totalSpendable = currentBal + salaryTotal - billsTotal;
    double savingAmount = billsTotal;

    cout << "TOTAL AMOUNT THAT CAN BE SPENT: Rs " << totalSpendable << "\n";
    cout << "TOTAL AMOUNT THAT SHOULD BE SAVED (SAVING'S AMOUNT): Rs " << savingAmount << "\n";

    //Savings transfer check
    cout << "Transfer savings to savings account? (y/n): ";
    char r;
    cin >> r;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (r == 'y' || r == 'Y') {
        cout << "Enter savings account number: ";
        string a1, a2;
        cin >> a1;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Re-enter: ";
        cin >> a2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (a1 != a2) {
            cout << "Mismatch.\n";
            printLine("SPENDING");
            return;
        }

        //Find customer
        Customer* t = reg.getCustomerByAccountNo(a1);
        if (!t) {
            cout << "Account not found.\n";
            cout << "Create one? (1=yes 0=no): ";
            if (readInt() == 1)
                cout << "Return to main menu.\n";
            printLine("SPENDING");
            return;
        }

        //Verify savings account
        if (!dynamic_cast<SavingsAccount*>(t->account)) {
            cout << "That is not a savings account.\n";
            printLine("SPENDING");
            return;
        }

        cout << "Savings verified (DISPLAY ONLY). No money transferred.\n";
    }

    printLine("SPENDING");
}


//===========================================================
// MAIN BUDGET MENU
//===========================================================

//Main budget dashboard
void BudgetManager::showBudgetMenu(Customer* cust, UserRegistration& reg) {
    if (!cust) return;

    while (true) {
        cout << "\nBUDGET DASHBOARD\n";
        printLine("BUDGET DASHBOARD");

        //Menu
        cout << "1. Bills\n";
        cout << "2. Salary\n";
        cout << "3. Spending\n";
        cout << "4. Back\n";
        printLine("BUDGET DASHBOARD");

        cout << "Choice: ";
        int ch = readInt();

        //Bills section
        if (ch == 1) {
            cout << "\nBILLS MENU\n";
            printLine("BILLS MENU");

            cout << "1. Add bills\n";
            cout << "2. Update bills\n";
            cout << "3. View bills\n";
            cout << "4. Back\n";

            cout << "Choice: ";
            int b = readInt();

            if      (b == 1) addBill(cust);
            else if (b == 2) updateBills(cust);
            else if (b == 3) viewBills(cust);
        }
        //Salary section
        else if (ch == 2) {
            addOrUpdateSalary(cust);
        }
        //Spending section
        else if (ch == 3) {
            spendingMenu(cust, reg);
        }
        else {
            break;
        }
    }
}
