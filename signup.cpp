#include "signup.h"
#include "transaction.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

//Initialize static user map
map<string, Customer> UserRegistration::users;

//Customer constructor
Customer::Customer()
    : isLocked(false), failedAttempts(0), lockTime(0), account(nullptr) {}


//===========================================================
// PASSWORD HASH AND SIMPLE ENCRYPTION
//===========================================================

//Hash password using simple function
string Customer::hashPassword(const string& password) {
    unsigned int hash = 5381;
    for (char c : password)
        hash = ((hash << 5) + hash) ^ c;
    stringstream ss; ss << hex << hash;
    return ss.str();
}

//Encrypt or decrypt using XOR
string Customer::encryptDecrypt(const string& data) {
    char key = 'K';
    string result = data;
    for (size_t i = 0; i < data.size(); i++)
        result[i] = data[i] ^ key;
    return result;
}

//Set password by hashing
void Customer::setPassword(string pass) { passwordHash = hashPassword(pass); }

//Check password match
bool Customer::checkPassword(const string& pass) {
    return passwordHash == hashPassword(pass);
}

//Get account number if available
string Customer::getBankAccountNo() const {
    return account ? account->getAccNum() : "";
}


//===========================================================
// ACCOUNT UPGRADE OR DOWNGRADE
//===========================================================

//Upgrade or downgrade subscription tier
void Customer::upgradeAccount() {
    cout << "\n--- SUBSCRIPTION OPTIONS ---\n";

    bool isSavings = dynamic_cast<SavingsAccount*>(account) != nullptr;

    //Menu for savings accounts
    if (isSavings) {
        cout << "You have a SAVINGS account.\n";
        cout << "1. Upgrade to Savings Silver\n";
        cout << "2. Upgrade to Savings Gold\n";
        cout << "3. Cancel Subscription (Downgrade to Savings Basic)\n";
    } 
    //Menu for normal accounts
    else {
        cout << "You have a NORMAL account.\n";
        cout << "1. Upgrade to Silver\n";
        cout << "2. Upgrade to Gold\n";
        cout << "3. Cancel Subscription (Downgrade to Basic)\n";
    }

    cout << "Enter choice: ";
    int ch;
    cin >> ch;

    string acc = account->getAccNum();
    double bal = account->getBal();

    //Lambda to enforce minimum balance
    auto enforceMinBalance = [&](int minBal) {
        if (bal < minBal) {
            cout << "\nYour current balance (" << bal << ") is below the minimum (" 
                 << minBal << ").\n";
            cout << "You must deposit at least " << (minBal - bal) << ".\n";

            double dep = 0;
            do {
                cout << "Enter deposit: ";
                cin >> dep;
                if (dep < (minBal - bal))
                    cout << "Deposit not enough.\n";
            } while (dep < (minBal - bal));

            bal += dep;
            account->setBal(bal);
            cout << "Deposit successful. New balance: " << bal << "\n";
        }
    };

    //Handle savings account tier change
    if (isSavings) {
        if (ch == 1) {
            enforceMinBalance(SavingsSilver::MinBal);
            delete account;
            account = new SavingsSilver(acc, bal);
            cout << "Upgraded to SAVINGS SILVER.\n";
        }
        else if (ch == 2) {
            enforceMinBalance(SavingsGold::MinBal);
            delete account;
            account = new SavingsGold(acc, bal);
            cout << "Upgraded to SAVINGS GOLD.\n";
        }
        else if (ch == 3) {
            delete account;
            account = new SavingsBasic(acc, bal);
            cout << "Downgraded to SAVINGS BASIC.\n";
        }
        else cout << "Invalid choice.\n";

        return;
    }

    //Handle normal account tier change
    if (ch == 1) {
        enforceMinBalance(SilverAccount::MinBal);
        delete account;
        account = new SilverAccount(acc, bal);
        cout << "Upgraded to SILVER.\n";
    }
    else if (ch == 2) {
        enforceMinBalance(GoldAccount::MinBal);
        delete account;
        account = new GoldAccount(acc, bal);
        cout << "Upgraded to GOLD.\n";
    }
    else if (ch == 3) {
        delete account;
        account = new BasicAccount(acc, bal);
        cout << "Downgraded to BASIC.\n";
    }
    else {
        cout << "Invalid choice.\n";
    }
}


//===========================================================
// REGISTER NEW USER
//===========================================================

//Registration process for new users
bool UserRegistration::registerUser(const string& username, const string&, const string& accNo) {

    //Check if username exists
    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
        return false;
    }

    //Full name input
    string fullName;
    cout << "Enter Full Name (First Last): ";
    cin.ignore();
    getline(cin, fullName);
    while (fullName.find(' ') == string::npos) {
        cout << "Enter name in correct format: ";
        getline(cin, fullName);
    }

    //Phone number input
    string phone;
    cout << "Enter 10-digit phone number: ";
    cin >> phone;
    while (phone.size() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit)) {
        cout << "Invalid phone. Re-enter: ";
        cin >> phone;
    }

    //Password input
    string p1, p2;
    cout << "Enter password (1 upper, 1 lower, 1 digit, 8+ chars): ";
    cin >> p1;

    //Password validation lambda
    auto validPassword = [&](string p) {
        if (p.length() < 8) return false;
        bool up=0, low=0, dig=0;
        for (char c: p) {
            if (isupper(c)) up=1;
            if (islower(c)) low=1;
            if (isdigit(c)) dig=1;
        }
        return up && low && dig;
    };

    //Ensure password meets rules
    while (!validPassword(p1)) {
        cout << "Invalid. Re-enter: ";
        cin >> p1;
    }

    cout << "Re-enter password: ";
    cin >> p2;

    if (p1 != p2) {
        cout << "Passwords mismatch. Registration cancelled.\n";
        return false;
    }

    //Create customer object
    Customer c;
    c.setPassword(p1);
    c.fullName = fullName;
    c.phoneNumber = phone;

    //Account type and tier
    printTierInfo();
    int type, tier;
    cout << "Account type (1=Normal, 2=Savings): ";
    cin >> type;
    cout << "Tier (1=Basic, 2=Silver, 3=Gold): ";
    cin >> tier;

    //Assign account based on selection
    if (type == 2) {
        if (tier == 1) c.account = new SavingsBasic(accNo, 0);
        else if (tier == 2) c.account = new SavingsSilver(accNo, 0);
        else c.account = new SavingsGold(accNo, 0);
    } else {
        if (tier == 1) c.account = new BasicAccount(accNo, 0);
        else if (tier == 2) c.account = new SilverAccount(accNo, 0);
        else c.account = new GoldAccount(accNo, 0);
    }

    //Minimum initial deposit check
    double minReq = 0;
    if (tier == 2) minReq = MIN_SILVER;
    else if (tier == 3) minReq = MIN_GOLD;

    if (minReq > 0) {
        double dep;
        do {
            cout << "Min initial deposit = " << minReq << ". Enter amount: ";
            cin >> dep;
        } while (dep < minReq);

        c.account->setBal(dep);
    }

    //Initialize salary and bills
    c.salary.amount = 0;
    c.salary.frequency = "none";

    //Store user
    users[username] = c;
    cout << "User Registered Successfully.\n";
    return true;
}


//===========================================================
// AUTHENTICATE USER
//===========================================================

//Login authentication
bool UserRegistration::authenticateUser(const string& username, const string& password) {
    Customer* c = getCustomerByUsername(username);
    if (!c) {
        cout << "User not found.\n";
        return false;
    }

    //Check lock status
    if (c->isLocked) {
        time_t now = time(nullptr);
        if (difftime(now, c->lockTime) >= LOCK_DURATION) {
            c->isLocked = false;
            c->failedAttempts = 0;
        } else {
            cout << "Account locked. Try later.\n";
            return false;
        }
    }

    //Check password match
    if (c->checkPassword(password)) {
        c->failedAttempts = 0;
        return true;
    }

    //Handle failed attempts
    cout << "Incorrect password.\n";
    if (++c->failedAttempts >= MAX_FAILED_ATTEMPTS) {
        c->isLocked = true;
        c->lockTime = time(nullptr);
        cout << "Account locked.\n";
    }
    return false;
}


//===========================================================
// GETTERS
//===========================================================

//Get user by username
Customer* UserRegistration::getCustomerByUsername(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) return &it->second;
    return nullptr;
}

//Get user by account number
Customer* UserRegistration::getCustomerByAccountNo(const string& accNo) {
    for (auto& p : users)
        if (p.second.getBankAccountNo() == accNo)
            return &p.second;
    return nullptr;
}


//===========================================================
// DELETE USER
//===========================================================

//Delete a user account
bool UserRegistration::deleteUserByUsername(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        delete it->second.account;
        users.erase(it);
        cout << "User deleted.\n";
        return true;
    }
    cout << "User not found.\n";
    return false;
}


//===========================================================
// SAVE USERS TO FILE (INCLUDES BILLS AND SALARY)
//===========================================================

//Save users to encrypted file
bool UserRegistration::saveUsersToFile(const string& filename) {
    try {
        ofstream ofs(filename, ios::binary);
        if (!ofs) return false;

        stringstream ss;

        //Write each user
        for (auto& p : users) {
            Customer& c = p.second;

            //Account type flag
            char type = (dynamic_cast<SavingsAccount*>(c.account) ? 'S' : 'N');

            //Account tier
            int tier = 1;
            if (dynamic_cast<SilverAccount*>(c.account) || dynamic_cast<SavingsSilver*>(c.account)) tier = 2;
            else if (dynamic_cast<GoldAccount*>(c.account) || dynamic_cast<SavingsGold*>(c.account)) tier = 3;

            //Basic data
            ss << p.first << ","
               << c.passwordHash << ","
               << c.getBankAccountNo() << ","
               << type << ","
               << tier << ","
               << c.account->getBal() << ","
               << c.fullName << ","
               << c.phoneNumber << ",";

            //Bill data
            ss << c.bills.size() << ",";
            for (auto& b : c.bills) {
                ss << b.name << "|" << b.amount << "|" << b.frequency << ";";
            }
            ss << ",";

            //Salary data
            ss << c.salary.amount << "," 
               << c.salary.frequency << "\n";
        }

        //Encrypt and save
        string encrypted = Customer::encryptDecrypt(ss.str());
        ofs.write(encrypted.c_str(), encrypted.size());
        ofs.close();
        return true;
    }
    catch (...) {
        return false;
    }
}


//===========================================================
// LOAD USERS FROM FILE (INCLUDES BILLS AND SALARY)
//===========================================================

//Load users from encrypted file
bool UserRegistration::loadUsersFromFile(const string& filename) {
    try {
        ifstream ifs(filename, ios::binary);
        if (!ifs) return false;

        string encrypted((istreambuf_iterator<char>(ifs)), {});
        ifs.close();

        //Decrypt file
        string data = Customer::encryptDecrypt(encrypted);
        stringstream ss(data);
        string line;

        users.clear();

        //Process file line by line
        while (getline(ss, line)) {
            if (line.empty()) continue;

            stringstream l(line);
            string user, pass, acc, tStr, tierStr, balStr, nameStr, phoneStr;
            string billsCountStr, billsDataStr, salAmtStr, salFreqStr;

            //Parse fields
            getline(l, user, ',');
            getline(l, pass, ',');
            getline(l, acc, ',');
            getline(l, tStr, ',');
            getline(l, tierStr, ',');
            getline(l, balStr, ',');
            getline(l, nameStr, ',');
            getline(l, phoneStr, ',');

            getline(l, billsCountStr, ',');
            getline(l, billsDataStr, ',');
            getline(l, salAmtStr, ',');
            getline(l, salFreqStr, ',');

            //Create customer
            Customer c;
            c.passwordHash = pass;
            c.fullName = nameStr;
            c.phoneNumber = phoneStr;

            bool isSavings = (tStr[0] == 'S');
            int tier = stoi(tierStr);
            double bal = stod(balStr);

            //Recreate account
            if (isSavings) {
                if (tier == 1) c.account = new SavingsBasic(acc, bal);
                else if (tier == 2) c.account = new SavingsSilver(acc, bal);
                else c.account = new SavingsGold(acc, bal);
            } else {
                if (tier == 1) c.account = new BasicAccount(acc, bal);
                else if (tier == 2) c.account = new SilverAccount(acc, bal);
                else c.account = new GoldAccount(acc, bal);
            }

            //Load bills
            int billCount = stoi(billsCountStr);
            stringstream bss(billsDataStr);
            string entry;

            while (getline(bss, entry, ';')) {
                if (entry.empty()) continue;

                stringstream one(entry);
                string name, amountStr, freqStr;

                getline(one, name, '|');
                getline(one, amountStr, '|');
                getline(one, freqStr, '|');

                Bill bill;
                bill.name = name;
                bill.amount = stod(amountStr);
                bill.frequency = freqStr;

                c.bills.push_back(bill);
            }

            //Load salary
            c.salary.amount = stod(salAmtStr);
            c.salary.frequency = salFreqStr;

            users[user] = c;
        }

        return true;
    }
    catch (...) {
        return false;
    }
}


//===========================================================
// UPDATE PHONE NUMBER
//===========================================================

//Change stored phone number
bool UserRegistration::updatePhoneNumber(const string& username, const string& newPhone) {
    if (newPhone.size() != 10 || !all_of(newPhone.begin(), newPhone.end(), ::isdigit))
        return false;

    Customer* c = getCustomerByUsername(username);
    if (!c) return false;

    c->phoneNumber = newPhone;
    return true;
}
