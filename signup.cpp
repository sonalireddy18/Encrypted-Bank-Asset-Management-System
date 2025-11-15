#include "signup.h"
#include "transaction.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

// static map initialization
map<string, Customer> UserRegistration::users;

Customer::Customer()
    : isLocked(false), failedAttempts(0), lockTime(0), account(nullptr) {}


// -----------------------------------------------------
// PASSWORD HASH + ENCRYPTION
// -----------------------------------------------------
string Customer::hashPassword(const string& password) {
    unsigned int hash = 5381;
    for (char c : password)
        hash = ((hash << 5) + hash) ^ c;
    stringstream ss; ss << hex << hash;
    return ss.str();
}

string Customer::encryptDecrypt(const string& data) {
    char key = 'K';
    string result = data;
    for (size_t i = 0; i < data.size(); i++)
        result[i] = data[i] ^ key;
    return result;
}

void Customer::setPassword(string pass) { passwordHash = hashPassword(pass); }

bool Customer::checkPassword(const string& pass) {
    return passwordHash == hashPassword(pass);
}

string Customer::getBankAccountNo() const {
    return account ? account->getAccNum() : "";
}


// -----------------------------------------------------
// 🔥🔥 FINAL UPGRADE ACCOUNT IMPLEMENTATION (Option C)
// -----------------------------------------------------
void Customer::upgradeAccount() {
    cout << "\n--- SUBSCRIPTION OPTIONS ---\n";

    bool isSavings = dynamic_cast<SavingsAccount*>(account) != nullptr;

    if (isSavings) {
        cout << "You have a SAVINGS account.\n";
        cout << "1. Upgrade to Savings Silver\n";
        cout << "2. Upgrade to Savings Gold\n";
        cout << "3. Cancel Subscription (Downgrade to Savings Basic)\n";
    } else {
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

    // Helper to enforce minimum balance WITH forced deposit
    auto enforceMinBalance = [&](int minBal) {
        if (bal < minBal) {
            cout << "\nYour current balance (" << bal << ") is below the required minimum (" 
                 << minBal << ").\n";
            cout << "You must deposit at least " << (minBal - bal) << " to continue.\n";

            double dep = 0;
            do {
                cout << "Enter deposit amount: ";
                cin >> dep;
                if (dep < (minBal - bal)) {
                    cout << "Deposit not enough. NOT DEPOSITED. Please deposit at least " 
                         << (minBal - bal) << ".\n";
                }
            } while (dep < (minBal - bal));

            bal += dep;
            account->setBal(bal);
            cout << "Deposit successful. New balance: " << bal << "\n";
        }
    };

    // -----------------------------------------------------
    // SAVINGS ACCOUNT HANDLING
    // -----------------------------------------------------
    if (isSavings) {

        if (ch == 1) {  // Savings → Silver
            enforceMinBalance(SavingsSilver::MinBal);
            delete account;
            account = new SavingsSilver(acc, bal);
            cout << "Account upgraded to SAVINGS SILVER.\n";
        }
        else if (ch == 2) {  // Savings → Gold
            enforceMinBalance(SavingsGold::MinBal);
            delete account;
            account = new SavingsGold(acc, bal);
            cout << "Account upgraded to SAVINGS GOLD.\n";
        }
        else if (ch == 3) {  // Cancel → Savings Basic
            delete account;
            account = new SavingsBasic(acc, bal);
            cout << "Subscription cancelled. You are now on SAVINGS BASIC.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
        return;
    }

    // -----------------------------------------------------
    // NORMAL ACCOUNT HANDLING
    // -----------------------------------------------------
    if (ch == 1) {  // Normal → Silver
        enforceMinBalance(SilverAccount::MinBal);
        delete account;
        account = new SilverAccount(acc, bal);
        cout << "Account upgraded to SILVER.\n";
    }
    else if (ch == 2) {  // Normal → Gold
        enforceMinBalance(GoldAccount::MinBal);
        delete account;
        account = new GoldAccount(acc, bal);
        cout << "Account upgraded to GOLD.\n";
    }
    else if (ch == 3) {  // Cancel → Basic
        delete account;
        account = new BasicAccount(acc, bal);
        cout << "Subscription cancelled. You are now on BASIC.\n";
    }
    else {
        cout << "Invalid choice.\n";
    }
}


// -----------------------------------------------------
// REGISTER USER
// -----------------------------------------------------
bool UserRegistration::registerUser(const string& username, const string&, const string& accNo) {

    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
        return false;
    }

    // FULL NAME
    string fullName;
    cout << "Enter Full Name (First Last): ";
    cin.ignore();
    getline(cin, fullName);
    while (fullName.find(' ') == string::npos) {
        cout << "Enter name in correct format: ";
        getline(cin, fullName);
    }

    // PHONE
    string phone;
    cout << "Enter 10-digit phone number: ";
    cin >> phone;
    while (phone.size() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit)) {
        cout << "Invalid phone. Re-enter: ";
        cin >> phone;
    }

    // PASSWORD
    string p1, p2;
    cout << "Enter password (1 upper, 1 lower, 1 digit, 8+ chars): ";
    cin >> p1;

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

    // CREATE CUSTOMER
    Customer c;
    c.setPassword(p1);
    c.fullName = fullName;
    c.phoneNumber = phone;

    // ACCOUNT TYPE
    printTierInfo();
    int type, tier;
    cout << "Account type (1=Normal, 2=Savings): ";
    cin >> type;
    cout << "Tier (1=Basic, 2=Silver, 3=Gold): ";
    cin >> tier;

    if (type == 2) {
        if (tier == 1) c.account = new SavingsBasic(accNo, 0);
        else if (tier == 2) c.account = new SavingsSilver(accNo, 0);
        else c.account = new SavingsGold(accNo, 0);
    } else {
        if (tier == 1) c.account = new BasicAccount(accNo, 0);
        else if (tier == 2) c.account = new SilverAccount(accNo, 0);
        else c.account = new GoldAccount(accNo, 0);
    }

    // MINIMUM INITIAL DEPOSIT
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

    users[username] = c;
    cout << "User Registered Successfully.\n";
    return true;
}


// -----------------------------------------------------
// AUTHENTICATION
// -----------------------------------------------------
bool UserRegistration::authenticateUser(const string& username, const string& password) {
    Customer* c = getCustomerByUsername(username);
    if (!c) {
        cout << "User not found.\n";
        return false;
    }

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

    if (c->checkPassword(password)) {
        c->failedAttempts = 0;
        return true;
    }

    cout << "Incorrect password.\n";
    if (++c->failedAttempts >= MAX_FAILED_ATTEMPTS) {
        c->isLocked = true;
        c->lockTime = time(nullptr);
        cout << "Account locked.\n";
    }
    return false;
}


// -----------------------------------------------------
// GETTERS
// -----------------------------------------------------
Customer* UserRegistration::getCustomerByUsername(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) return &it->second;
    return nullptr;
}

Customer* UserRegistration::getCustomerByAccountNo(const string& accNo) {
    for (auto& p : users)
        if (p.second.getBankAccountNo() == accNo)
            return &p.second;
    return nullptr;
}


// -----------------------------------------------------
// DELETE USER
// -----------------------------------------------------
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


// -----------------------------------------------------
// SAVE USERS TO FILE
// -----------------------------------------------------
bool UserRegistration::saveUsersToFile(const string& filename) {
    try {
        ofstream ofs(filename, ios::binary);
        if (!ofs) return false;

        stringstream ss;

        for (auto& p : users) {
            Customer& c = p.second;

            char type = (dynamic_cast<SavingsAccount*>(c.account) ? 'S' : 'N');

            int tier = 1;
            if (dynamic_cast<SilverAccount*>(c.account) || dynamic_cast<SavingsSilver*>(c.account)) tier = 2;
            else if (dynamic_cast<GoldAccount*>(c.account) || dynamic_cast<SavingsGold*>(c.account)) tier = 3;

            ss << p.first << ","
               << c.passwordHash << ","
               << c.getBankAccountNo() << ","
               << type << ","
               << tier << ","
               << c.account->getBal() << ","
               << c.fullName << ","
               << c.phoneNumber << "\n";
        }

        string encrypted = Customer::encryptDecrypt(ss.str());
        ofs.write(encrypted.c_str(), encrypted.size());
        ofs.close();
        return true;
    }
    catch (...) {
        return false;
    }
}


// -----------------------------------------------------
// LOAD USERS FROM FILE
// -----------------------------------------------------
bool UserRegistration::loadUsersFromFile(const string& filename) {
    try {
        ifstream ifs(filename, ios::binary);
        if (!ifs) return false;

        string encrypted((istreambuf_iterator<char>(ifs)), {});
        ifs.close();

        string data = Customer::encryptDecrypt(encrypted);

        stringstream ss(data);
        string line;

        users.clear();

        while (getline(ss, line)) {
            if (line.empty()) continue;

            stringstream l(line);
            string user, pass, acc, tStr, tierStr, balStr, nameStr, phoneStr;

            getline(l, user, ',');
            getline(l, pass, ',');
            getline(l, acc, ',');
            getline(l, tStr, ',');
            getline(l, tierStr, ',');
            getline(l, balStr, ',');
            getline(l, nameStr, ',');
            getline(l, phoneStr, ',');

            Customer c;
            c.passwordHash = pass;
            c.fullName = nameStr;
            c.phoneNumber = phoneStr;

            bool isSavings = (tStr[0] == 'S');
            int tier = stoi(tierStr);
            double bal = stod(balStr);

            if (isSavings) {
                if (tier == 1) c.account = new SavingsBasic(acc, bal);
                else if (tier == 2) c.account = new SavingsSilver(acc, bal);
                else c.account = new SavingsGold(acc, bal);
            } else {
                if (tier == 1) c.account = new BasicAccount(acc, bal);
                else if (tier == 2) c.account = new SilverAccount(acc, bal);
                else c.account = new GoldAccount(acc, bal);
            }

            users[user] = c;
        }

        return true;
    }
    catch (...) {
        return false;
    }
}


// -----------------------------------------------------
// UPDATE PHONE NUMBER
// -----------------------------------------------------
bool UserRegistration::updatePhoneNumber(const string& username, const string& newPhone) {
    if (newPhone.size() != 10 || !all_of(newPhone.begin(), newPhone.end(), ::isdigit))
        return false;

    Customer* c = getCustomerByUsername(username);
    if (!c) return false;

    c->phoneNumber = newPhone;
    return true;
}
