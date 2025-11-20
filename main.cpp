#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include "signup.h"
#include "transaction.h"
#include "account.h"
#include "block.h"
#include "budget.h"

using namespace std;

//Print a line underline
static void printLine(const string& text) {
    cout << string(text.size(), '-') << "\n";
}

//Show main menu before login
void showMainMenu() {
    string title = "BANK ASSET MANAGEMENT SYSTEM (BAMS)";
    cout << "\n" << title << "\n";
    printLine(title);
    cout << "1. Register New User\n";
    cout << "2. Login to Existing Account\n";
    cout << "3. Exit\n";
    printLine(title);
    cout << endl;
}

//Show user menu with dynamic title
void showUserMenu(const string& titleText) {
    string title = titleText;
    cout << "\n" << title << "\n";
    printLine(title);
    cout << "1. Transfer by Account Number\n";
    cout << "2. Deposit\n";
    cout << "3. Withdraw\n";
    cout << "4. View Balance\n";
    cout << "5. Print Transaction History\n";
    cout << "6. Delete Account\n";
    cout << "7. Logout\n";
    cout << "8. Upgrade / Cancel Subscription\n";
    cout << "9. View Blockchain Ledger\n";
    cout << "10. Budget\n"; 
    printLine(title);
    cout << endl;
}

//Main program loop
int main() {
    UserRegistration registrationSystem;
    Transaction transactionSystem;
    BudgetManager budget;
    string currentUsername;
    bool loggedIn = false;

    //Load stored data
    registrationSystem.loadUsersFromFile("users.dat");
    Transaction::loadBlockchainFromFile("transactions.dat");

    int choice = 0;

    //Main loop
    while (true) {

        //===========================================================
        // USER NOT LOGGED IN
        //===========================================================
        if (!loggedIn) {
            showMainMenu();
            cout << "Enter your choice: ";

            //Validate input
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string err = "Invalid input. Please enter a number.";
                cout << err << "\n";
                printLine(err);
                continue;
            }

            //Register new user
            if (choice == 1) {
                string username, accNo;

                cout << "\nREGISTER NEW USER\n";
                printLine("REGISTER NEW USER");

                cout << "Enter username: ";
                cin >> username;
                cout << "Enter Account Number: ";
                cin >> accNo;

                registrationSystem.registerUser(username, "", accNo);

                printLine("REGISTER NEW USER");
            }

            //Login
            else if (choice == 2) {
                string username, password;

                cout << "\nUSER LOGIN\n";
                printLine("USER LOGIN");

                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if (registrationSystem.authenticateUser(username, password)) {
                    cout << "Login Successful!\n";
                    printLine("Login Successful!");
                    currentUsername = username;
                    loggedIn = true;
                } 
                else {
                    cout << "Login Failed!\n";
                    printLine("Login Failed!");
                }
            }

            //Exit system
            else if (choice == 3) {
                cout << "\nSaving users and blockchain...\n";
                registrationSystem.saveUsersToFile("users.dat");
                Transaction::saveBlockchainToFile("transactions.dat");
                cout << "Exiting System...\n";
                printLine("Exiting System...");
                break;
            }

            //Invalid choice
            else {
                cout << "Invalid Choice!\n";
                printLine("Invalid Choice!");
            }
        }

        //===========================================================
        // USER LOGGED IN
        //===========================================================
        else {

            //Get current customer
            Customer* cust = registrationSystem.getCustomerByUsername(currentUsername);
            if (!cust) {
                cout << "User session error. Logging out...\n";
                loggedIn = false;
                continue;
            }

            //===========================================================
            // BUILD DYNAMIC DASHBOARD TITLE
            //===========================================================
            string accType;

            if (dynamic_cast<GoldAccount*>(cust->account) || dynamic_cast<SavingsGold*>(cust->account))
                accType = "GOLD";
            else if (dynamic_cast<SilverAccount*>(cust->account) || dynamic_cast<SavingsSilver*>(cust->account))
                accType = "SILVER";
            else
                accType = "BASIC";

            string firstName = cust->fullName;
            size_t pos = firstName.find(' ');
            if (pos != string::npos)
                firstName = firstName.substr(0, pos);

            string dynamicTitle = accType + " " + firstName + "'S DASHBOARD";

            //Show user menu
            showUserMenu(dynamicTitle);

            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                continue;
            }

            string accNo = cust->getBankAccountNo();


            //===========================================================
            // USER MENU OPERATIONS
            //===========================================================

            //Transfer funds
            if (choice == 1) {
                string toAccNo; 
                double amount;

                cout << "\nTRANSFER FUNDS\n";
                printLine("TRANSFER FUNDS");

                cout << "Enter recipient account number: ";
                cin >> toAccNo;
                cout << "Enter amount to transfer: ";
                cin >> amount;

                transactionSystem.transferByAccountNoUserRegistration(
                    registrationSystem, accNo, toAccNo, amount
                );

                printLine("TRANSFER FUNDS");
            }

            //Deposit
            else if (choice == 2) {
                double amount;

                cout << "\nDEPOSIT MONEY\n";
                printLine("DEPOSIT MONEY");

                cout << "Enter amount to deposit: ";
                cin >> amount;

                transactionSystem.depositUserRegistration(registrationSystem, accNo, amount);

                printLine("DEPOSIT MONEY");
            }

            //Withdraw
            else if (choice == 3) {
                double amount;

                cout << "\nWITHDRAW MONEY\n";
                printLine("WITHDRAW MONEY");

                cout << "Enter amount to withdraw: ";
                cin >> amount;

                transactionSystem.withdrawUserRegistration(registrationSystem, accNo, amount);

                printLine("WITHDRAW MONEY");
            }

            //View balance
            else if (choice == 4) {
                cout << "\nACCOUNT BALANCE\n";
                printLine("ACCOUNT BALANCE");

                cout << "Current Balance: Rs "
                     << fixed << setprecision(2)
                     << cust->account->getBal() << "\n";

                printLine("ACCOUNT BALANCE");
            }

            //Print history
            else if (choice == 5) {
                cout << "\nTRANSACTION HISTORY\n";
                printLine("TRANSACTION HISTORY");

                transactionSystem.printTransactionHistory(accNo);

                printLine("TRANSACTION HISTORY");
            }

            //Delete account
            else if (choice == 6) {
                cout << "\nDELETE ACCOUNT\n";
                printLine("DELETE ACCOUNT");

                cout << "Are you sure you want to delete your account? (y/n): ";
                char confirm;
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    registrationSystem.deleteUserByUsername(currentUsername);
                    loggedIn = false;
                    currentUsername.clear();
                }

                printLine("DELETE ACCOUNT");
            }

            //Logout
            else if (choice == 7) {
                cout << "Logging out...\n";
                loggedIn = false;
                currentUsername.clear();
            }

            //Upgrade account
            else if (choice == 8) {
                cout << "\nACCOUNT UPGRADE\n";
                printLine("ACCOUNT UPGRADE");

                cust->upgradeAccount();

                printLine("ACCOUNT UPGRADE");
            }

            //Show blockchain
            else if (choice == 9) {
                cout << "\nBLOCKCHAIN LEDGER\n";
                printLine("BLOCKCHAIN LEDGER");

                Transaction::printFullBlockchain();

                printLine("BLOCKCHAIN LEDGER");
            }

            //Budget menu
            else if (choice == 10) { 
                budget.showBudgetMenu(cust, registrationSystem);
            }

            //Invalid choice
            else {
                cout << "Invalid Choice!\n";
                printLine("Invalid Choice!");
            }
        }
    }

    return 0;
}
