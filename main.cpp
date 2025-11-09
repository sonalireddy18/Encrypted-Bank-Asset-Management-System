#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include "signup.h"
#include "transaction.h"
#include "account.h"
#include "block.h"
using namespace std;

// Local helper for neat separators
static void printLine(const string& text) {
    cout << string(text.size(), '-') << "\n";
}

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

void showUserMenu() {
    string title = "USER DASHBOARD";
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
    printLine(title);
    cout << endl;
}

int main() {
    UserRegistration registrationSystem;
    Transaction transactionSystem;
    string currentUsername;
    bool loggedIn = false;

    // Load users and blockchain (previous transaction history)
    registrationSystem.loadUsersFromFile("users.dat");
    Transaction::loadBlockchainFromFile("transactions.dat");

    int choice = 0;
    while (true) {
        if (!loggedIn) {
            showMainMenu();
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string err = "Invalid input. Please enter a number.";
                cout << err << "\n";
                printLine(err);
                continue;
            }

            if (choice == 1) {
                string username, passwordDummy, accNo;
                string header = "REGISTER NEW USER";
                cout << "\n" << header << "\n";
                printLine(header);

                cout << "Enter username: ";
                cin >> username;
                cout << "Enter Account Number: ";
                cin >> accNo;

                registrationSystem.registerUser(username, passwordDummy, accNo);
                printLine(header);
            } 
            else if (choice == 2) {
                string username, password;
                string header = "USER LOGIN";
                cout << "\n" << header << "\n";
                printLine(header);

                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if (registrationSystem.authenticateUser(username, password)) {
                    string msg = "Login Successful!";
                    cout << msg << "\n";
                    printLine(msg);
                    currentUsername = username;
                    loggedIn = true;
                } 
                else {
                    string msg = "Login Failed!";
                    cout << msg << "\n";
                    printLine(msg);
                }
            } 
            else if (choice == 3) {
                string msg = "Exiting System...";
                cout << "\nSaving users and blockchain...\n";
                // Save both users and transaction history
                registrationSystem.saveUsersToFile("users.dat");
                Transaction::saveBlockchainToFile("transactions.dat");

                cout << msg << "\n";
                printLine(msg);
                break;
            } 
            else {
                string msg = "Invalid Choice!";
                cout << msg << "\n";
                printLine(msg);
            }
        } 
        else {
            showUserMenu();
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string err = "Invalid input.";
                cout << err << "\n";
                printLine(err);
                continue;
            }

            Customer* cust = registrationSystem.getCustomerByUsername(currentUsername);
            if (!cust) {
                string msg = "User session error. Logging out...";
                cout << msg << "\n";
                printLine(msg);
                loggedIn = false;
                continue;
            }

            string accNo = cust->getBankAccountNo();

            if (choice == 1) {
                string toAccNo; double amount;
                string header = "TRANSFER FUNDS";
                cout << "\n" << header << "\n";
                printLine(header);
                cout << "Enter recipient account number: ";
                cin >> toAccNo;
                cout << "Enter amount to transfer: ";
                cin >> amount;
                transactionSystem.transferByAccountNoUserRegistration(registrationSystem, accNo, toAccNo, amount);
                printLine(header);
            } 
            else if (choice == 2) {
                double amount;
                string header = "DEPOSIT MONEY";
                cout << "\n" << header << "\n";
                printLine(header);
                cout << "Enter amount to deposit: ";
                cin >> amount;
                transactionSystem.depositUserRegistration(registrationSystem, accNo, amount);
                printLine(header);
            } 
            else if (choice == 3) {
                double amount;
                string header = "WITHDRAW MONEY";
                cout << "\n" << header << "\n";
                printLine(header);
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                transactionSystem.withdrawUserRegistration(registrationSystem, accNo, amount);
                printLine(header);
            } 
            else if (choice == 4) {
                string header = "ACCOUNT BALANCE";
                cout << "\n" << header << "\n";
                printLine(header);
                cout << "Current Balance: Rs " << fixed << setprecision(2) << cust->account->getBal() << "\n";
                printLine(header);
            } 
            else if (choice == 5) {
                string header = "TRANSACTION HISTORY";
                cout << "\n" << header << "\n";
                printLine(header);
                transactionSystem.printTransactionHistory();
                printLine(header);
            } 
            else if (choice == 6) {
                string header = "DELETE ACCOUNT";
                cout << "\n" << header << "\n";
                printLine(header);
                cout << "Are you sure you want to delete your account? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    registrationSystem.deleteUserByUsername(currentUsername);
                    loggedIn = false;
                    currentUsername.clear();
                }
                printLine(header);
            } 
            else if (choice == 7) {
                string msg = "Logging out...";
                cout << msg << "\n";
                printLine(msg);
                loggedIn = false;
                currentUsername.clear();
            } 
            else if (choice == 8) {
                string header = "ACCOUNT UPGRADE";
                cout << "\n" << header << "\n";
                printLine(header);
                cust->upgradeAccount();
                printLine(header);
            } 
            else {
                string msg = "Invalid Choice!";
                cout << msg << "\n";
                printLine(msg);
            }
        }
    }
    return 0;
}
