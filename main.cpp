#include <iostream>
#include <limits>
#include "signup.h"
using namespace std;
void showMenu() {
    cout << "===============================================\n";
    cout << "Welcome to BAMS - Bank Asset Management System\n";
    cout << "===============================================\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Enter choice: ";
}
int main() {
    UserRegistration reg;
    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                if (!reg.registerUser())
                    cout << "Registration failed.\n";
                break;
            case 2:
                if (!reg.loginUser())
                    cout << "Login failed.\n";
                else
                    cout << "Accessing BAMS secure dashboard...\n";
                break;
            case 3:
                cout << "Thank you for using BAMS!\n";
                break;
            default:
                cout << "Invalid choice!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
        }
        cout << endl;
    } while (choice != 3);

    return 0;
}
