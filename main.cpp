#include <iostream>
#include <limits>
#include "signup.h"
#include "Transaction.h"
#include "Accounts.h"

using namespace std;

// Function to display the main menu to the user
void showMainMenu() 
{
    cout << "\n===============================================\n";
    cout << "   Welcome to BAMS - Bank Asset Management System\n";
    cout << "===============================================\n";
    cout << "1. Register New User\n";
    cout << "2. Login to Existing Account\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

int main() 
{
    //Create an instance of UserRegistration to handle all user data
    UserRegistration registrationSystem;

    // Variable to store the user's menu selection
    int choice = 0; 

    // Infinite loop to keep showing the menu until user exits
    while (true) 
    {
        showMainMenu();

        // Validate input: check if the user actually entered a number
        if (!(cin >> choice)) 
        {
            // If input is invalid (e.g., a letter), reset cin and clear the buffer
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        // Process user's menu selection
        switch (choice) 
        {
            case 1:
                //Register a new user
                if (!registrationSystem.registerUser())
                    cout << "Registration failed. Please try again.\n";
                else
                    cout << "Registration successful!\n";
                break;

            case 2:
                //Login existing user
                if (!registrationSystem.loginUser())
                    cout << "Login failed. Please try again.\n";
                else
                    cout << "Returning to main menu...\n";
                break;

            case 3:
                //Exit the program
                cout << "Thank you for using BAMS. Goodbye!\n";
                return 0;  // Exit successfully

            default:
                //If user enters a number not in 1–3, show error
                cout << "Invalid option. Please choose between 1-3.\n";
        }

        cout << "\n";
    }

    return 0;
}
