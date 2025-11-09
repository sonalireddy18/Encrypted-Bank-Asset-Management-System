#include "signup.h"
#include "Transaction.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

map<string, Customer> UserRegistration::users;

Customer::Customer() 
    : isLocked(false), failedAttempts(0), lockTime(0), account(nullptr) {}

string Customer::hashPassword(const string& password) {
    unsigned int hash = 5381;
    for (char c : password) hash = ((hash << 5) + hash) ^ c;
    stringstream ss; ss << hex << hash;
    return ss.str();
}

string Customer::encryptDecrypt(const string& data) {
    char key = 'K';
    string result = data;
    for (size_t i = 0; i < data.size(); i++) result[i] = data[i] ^ key;
    return result;
}

void Customer::setPassword(string pass) { passwordHash = hashPassword(pass); }
bool Customer::checkPassword(const string& pass) { return passwordHash == hashPassword(pass); }
string Customer::getBankAccountNo() const { return account ? account->getAccNum() : ""; }

void Customer::upgradeAccount() {}  // already implemented elsewhere


//REGISTER USER
bool UserRegistration::registerUser(const string& username, const string&, const string& accNo) {
    if (users.find(username) != users.end()) {
        cout << "Username already exists.\n";
        return false;
    }

    // NAME input
    string fullName;
    cout << "Enter Full Name (First Last): ";
    cin.ignore();
    getline(cin, fullName);
    while (fullName.find(' ') == string::npos) {
        cout << "Invalid. Enter name as First Last: ";
        getline(cin, fullName);
    }

    // PHONE input
    string phone;
    cout << "Enter 10-digit phone number: ";
    cin >> phone;
    while (phone.size() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit)) {
        cout << "Invalid. Enter 10-digit numeric phone: ";
        cin >> phone;
    }

    // PASSWORD
    string p1,p2;
    cout<<"Password must contain 1 uppercase, 1 lowercase, 1 digit, min 8 chars\n";
    cout<<"Enter password: "; cin>>p1;
    auto validPassword=[&](string p){
        if(p.length()<8) return false;
        bool up=0,low=0,dig=0;
        for(char c:p){
            if(isupper(c)) up=1;
            if(islower(c)) low=1;
            if(isdigit(c)) dig=1;
        }
        return up && low && dig;
    };
    while(!validPassword(p1)){ cout<<"Invalid, try again: "; cin>>p1; }
    cout<<"Re-enter password: "; cin>>p2;
    if(p1!=p2){ cout<<"Mismatch. Cancelled.\n"; return false; }

    Customer c;
    c.setPassword(p1);
    c.fullName = fullName;
    c.phoneNumber = phone;

    printTierInfo();
    int type,tier;
    cout<<"Account type (1=Normal, 2=Savings): "; cin>>type;
    cout<<"Tier (1=Basic, 2=Silver, 3=Gold): "; cin>>tier;

    if(type == 2) {
        if(tier==1) c.account=new SavingsBasic(accNo,0);
        else if(tier==2) c.account=new SavingsSilver(accNo,0);
        else c.account=new SavingsGold(accNo,0);
    } else {
        if(tier==1) c.account=new BasicAccount(accNo,0);
        else if(tier==2) c.account=new SilverAccount(accNo,0);
        else c.account=new GoldAccount(accNo,0);
    }

    double minReq = 0;
    if(tier == 2) minReq = MIN_SILVER;
    else if(tier == 3) minReq = MIN_GOLD;

    if(minReq > 0){
        double dep;
        do{
            cout<<"Minimum initial deposit = "<<minReq<<". Enter amount: ";
            cin>>dep;
        } while(dep < minReq);
        c.account->setBal(dep);
    }

    users[username] = c;
    cout<<"User registered successfully.\n";
    return true;
}


//AUTHENTICATE USER
bool UserRegistration::authenticateUser(const string& username, const string& password) {
    Customer* c = getCustomerByUsername(username);
    if (!c) { cout << "User not found.\n"; return false; }

    if (c->isLocked) {
        time_t now = time(nullptr);
        if (difftime(now, c->lockTime) >= LOCK_DURATION) { c->isLocked=false; c->failedAttempts=0; }
        else { cout<<"Account locked.\n"; return false; }
    }

    if (c->checkPassword(password)) { c->failedAttempts=0; return true; }

    cout<<"Incorrect password.\n";
    if (++c->failedAttempts >= MAX_FAILED_ATTEMPTS) {
        c->isLocked = true;
        c->lockTime = time(nullptr);
        cout<<"Account locked.\n";
    }
    return false;
}

//GETTERS
Customer* UserRegistration::getCustomerByUsername(const string& username) {
    auto it = users.find(username);
    return (it != users.end()) ? &(it->second) : nullptr;
}
Customer* UserRegistration::getCustomerByAccountNo(const string& accNo) {
    for (auto& x : users)
        if (x.second.getBankAccountNo() == accNo)
            return &(x.second);
    return nullptr;
}

//DELETE USER
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

//SAVE USERS (now saves name + phone)
bool UserRegistration::saveUsersToFile(const string& filename) {
    try {
        ofstream ofs(filename, ios::binary);
        if (!ofs) return false;

        stringstream ss;

        for (auto &p : users) {
            Customer &c = p.second;
            char type = dynamic_cast<SavingsAccount*>(c.account) ? 'S' : 'N';

            int tier = 1;
            if(dynamic_cast<SilverAccount*>(c.account) || dynamic_cast<SavingsSilver*>(c.account)) tier=2;
            else if(dynamic_cast<GoldAccount*>(c.account) || dynamic_cast<SavingsGold*>(c.account)) tier=3;

            ss << p.first << "," << c.passwordHash << "," << c.getBankAccountNo() << ","
               << type << "," << tier << "," << c.account->getBal() << ","
               << c.fullName << "," << c.phoneNumber << "\n";
        }

        string encrypted = Customer::encryptDecrypt(ss.str());
        ofs.write(encrypted.c_str(), encrypted.size());
        ofs.close();
        return true;

    } catch (...) { return false; }
}

//LOAD USERS (reads name + phone)
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
            if(line.empty()) continue;

            stringstream l(line);
            string user, pass, acc, tStr, tierStr, balStr, nameStr, phoneStr;

            getline(l,user,',');
            getline(l,pass,',');
            getline(l,acc,',');
            getline(l,tStr,',');
            getline(l,tierStr,',');
            getline(l,balStr,',');
            getline(l,nameStr,',');
            getline(l,phoneStr,',');

            Customer c;
            c.passwordHash = pass;
            c.fullName = nameStr;
            c.phoneNumber = phoneStr;

            char type = tStr[0];
            int tier = stoi(tierStr);
            double bal = stod(balStr);

            if(type=='S'){
                if(tier==1) c.account = new SavingsBasic(acc,bal);
                else if(tier==2) c.account = new SavingsSilver(acc,bal);
                else c.account = new SavingsGold(acc,bal);
            } else {
                if(tier==1) c.account = new BasicAccount(acc,bal);
                else if(tier==2) c.account = new SilverAccount(acc,bal);
                else c.account = new GoldAccount(acc,bal);
            }

            users[user] = c;
        }
        return true;

    } catch (...) { return false; }
}


bool UserRegistration::updatePhoneNumber(const string& username, const string& newPhone) {
    if(newPhone.size()!=10 || !all_of(newPhone.begin(), newPhone.end(), ::isdigit))
        return false;

    auto *c = getCustomerByUsername(username);
    if(!c) return false;

    c->phoneNumber = newPhone;
    return true;
}
