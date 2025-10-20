#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <limits>
#include "signup.h"

using namespace std;

void showMenu() {
    cout << "===============================================" << endl;
    cout << "Welcome to BAMS - Bank Asset Management System" << endl;
    cout << "===============================================" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    UserRegistration reg;
    int choice;
    do {
        showMenu();
        cin >> choice;
        switch(choice) {
            case 1: {
                if (!reg.registerUser()) {
                    cout << "Registration failed. Please try again." << endl;
                }
                break;
            }
            case 2: {
                if (!reg.loginUser()) {
                    cout << "Login failed. Please try again." << endl;
                } else {
                    // Successful login - proceed to main banking application
                    cout << "\n=== Bank Asset Management System ===" << endl;
                    cout << "Login successful! Accessing your bank assets..." << endl;
                    // Here we will integrate with the blockchain transaction system
                    // and asset management features
                }
                break;
            }
            case 3: {
                cout << "Thank you for using BAMS. Goodbye!" << endl;
                break;
            }
            default: {
                cout << "Invalid choice! Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
        cout << endl;
    } while (choice != 3);
    
    return 0;
}
