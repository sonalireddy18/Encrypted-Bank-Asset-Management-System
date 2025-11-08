#include <iostream>
#include <limits>
#include <string>

#include "signup.h"
#include "transaction.h"
#include "Accounts.h"
#include "block.h"

using namespace std;

void showMainMenu() {
    cout << "Welcome to BAMS - Bank Asset Management System\n\n";
    cout << "1. Register New User\n";
    cout << "2. Login to Existing Account\n";
    cout << "3. Exit\n";
}

void showUserMenu() {
    cout << "\nUser Dashboard:\n";
    cout << "1. Transfer by Account Number\n";
    cout << "2. Deposit\n";
    cout << "3. Withdraw\n";
    cout << "4. View Balance\n";                     // ✅ Added feature
    cout << "5. Print Transaction History\n";
    cout << "6. Delete Account\n";
    cout << "7. Logout\n";
    cout << "8. Upgrade / Cancel Subscription\n";   // ✅ Added feature
}

int main() {
    UserRegistration registrationSystem;
    Transaction transactionSystem;
    string currentUsername;
    bool loggedIn = false;

    // ✅ Load users from file at startup
    registrationSystem.loadUsersFromFile("users.dat");

    int choice = 0;
    while (true) {
        if (!loggedIn) {
            showMainMenu();
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (choice == 1) {
                string username, passwordDummy, accNo;
                cout << "Enter username: "; cin >> username;
                cout << "Enter Account Number: "; cin >> accNo;

                registrationSystem.registerUser(username, passwordDummy, accNo);
            } 
            else if (choice == 2) {
                string username, password;
                cout << "Username: "; cin >> username;
                cout << "Password: "; cin >> password;

                if (registrationSystem.authenticateUser(username, password)) {
                    cout << "Login successful.\n";
                    currentUsername = username;
                    loggedIn = true;
                } 
                else {
                    cout << "Login failed.\n";
                }
            } 
            else if (choice == 3) {
                cout << "Saving users...\n";

                // ✅ Save users before exiting
                registrationSystem.saveUsersToFile("users.dat");

                cout << "Exiting...\n";
                break;
            } 
            else {
                cout << "Invalid choice.\n";
            }
        }

        else { // ✅ Logged in
            showUserMenu();
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                continue;
            }

            Customer* cust = registrationSystem.getCustomerByUsername(currentUsername);
            if (!cust) {
                cout << "User session error. Logging out.\n";
                loggedIn = false;
                continue;
            }

            string accNo = cust->getBankAccountNo();

            if (choice == 1) {
                string toAccNo;
                double amount;
                cout << "Enter recipient account number: "; cin >> toAccNo;
                cout << "Enter amount to transfer: "; cin >> amount;
                transactionSystem.transferByAccountNoUserRegistration(registrationSystem, accNo, toAccNo, amount);
            } 
            else if (choice == 2) {
                double amount;
                cout << "Enter amount to deposit: "; cin >> amount;
                transactionSystem.depositUserRegistration(registrationSystem, accNo, amount);
            } 
            else if (choice == 3) {
                double amount;
                cout << "Enter amount to withdraw: "; cin >> amount;
                transactionSystem.withdrawUserRegistration(registrationSystem, accNo, amount);
            } 
            else if (choice == 4) {
                // ✅ View Balance
                cout << "Current Balance: " << cust->account->getBal() << "\n";
            } 
            else if (choice == 5) {
                transactionSystem.printTransactionHistory();
            } 
            else if (choice == 6) {
                cout << "Are you sure you want to delete your account? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    registrationSystem.deleteUserByUsername(currentUsername);
                    loggedIn = false;
                    currentUsername.clear();
                }
            } 
            else if (choice == 7) {
                cout << "Logging out...\n";
                loggedIn = false;
                currentUsername.clear();
            }
            else if (choice == 8) {
                // ✅ Upgrade account
                cust->upgradeAccount();
            }
            else {
                cout << "Invalid choice.\n";
            }
        }
    }
    return 0;
}
