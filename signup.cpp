#include "signup.h"
#include <cctype>
#include <sstream>

// ==================== Customer Class ====================
Customer::Customer() : isLocked(false), failedAttempts(0), lockTime(0) {}
string Customer::hashPassword(const string& password) {
    unsigned int hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) ^ c;
    }
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}
string Customer::encryptDecrypt(const string& data) {
    char key = 'K';  // symmetric key for XOR
    string result = data;
    for (size_t i = 0; i < data.size(); ++i)
        result[i] = data[i] ^ key;
    return result;
}
void Customer::setPassword(string pass) {
    passwordHash = hashPassword(pass);
}
bool Customer::verifyPassword(string pass) const {
    string inputHash = hashPassword(pass);
    return passwordHash == inputHash;
}
bool Customer::isAccountLocked() const {
    if (isLocked) {
        time_t currentTime = time(nullptr);
        double hoursElapsed = difftime(currentTime, lockTime) / 3600.0;
        if (hoursElapsed >= 24) return false;
        return true;
    }
    return false;
}

void Customer::incrementFailedAttempts() {
    failedAttempts++;
    if (failedAttempts >= 3) {
        isLocked = true;
        lockTime = time(nullptr);
    }
}
void Customer::resetFailedAttempts() {
    failedAttempts = 0;
    isLocked = false;
}
void Customer::setBankAccountNo(string accNo) {
    bankAccountNo = accNo;
}
string Customer::getBankAccountNo() const {
    return bankAccountNo;
}
// File output (encryption applied) - FIXED: Proper line formatting
ofstream& operator<<(ofstream& ofs, const Customer& cust) {
    ofs << Customer::encryptDecrypt(cust.username) << "\n";
    ofs << Customer::encryptDecrypt(cust.name) << "\n";
    ofs << Customer::encryptDecrypt(cust.phno) << "\n";
    ofs << Customer::encryptDecrypt(cust.bankAccountNo) << "\n";
    ofs << cust.passwordHash << "\n";
    ofs << cust.failedAttempts << "\n";
    ofs << cust.isLocked << "\n";
    ofs << cust.lockTime << "\n";
    ofs << "---\n";  // Separator between users
    return ofs;
}
// File input (decryption applied) - FIXED: Proper line reading
ifstream& operator>>(ifstream& ifs, Customer& cust) {
    string line;
    
    // Read username
    if (!getline(ifs, line)) return ifs;
    cust.username = Customer::encryptDecrypt(line);

    // Read name
    if (!getline(ifs, line)) return ifs;
    cust.name = Customer::encryptDecrypt(line);

    // Read phone
    if (!getline(ifs, line)) return ifs;
    cust.phno = Customer::encryptDecrypt(line);

    // Read bank account
    if (!getline(ifs, line)) return ifs;
    cust.setBankAccountNo(Customer::encryptDecrypt(line));

    // Read password hash
    if (!getline(ifs, cust.passwordHash)) return ifs;

    // Read failed attempts
    if (!getline(ifs, line)) return ifs;
    cust.failedAttempts = stoi(line);

    // Read isLocked
    if (!getline(ifs, line)) return ifs;
    cust.isLocked = (line == "1");

    // Read lockTime
    if (!getline(ifs, line)) return ifs;
    cust.lockTime = stol(line);

    // Read separator
    if (!getline(ifs, line)) return ifs;

    return ifs;
}
// ==================== UserRegistration ====================
UserRegistration::UserRegistration() {
    loadFromFile();
}

UserRegistration::~UserRegistration() {
    saveToFile();
}

bool UserRegistration::isValidPassword(const string& password) const {
    if (password.length() < 8) return false;
    bool hasUpper = false, hasLower = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
    }
    return hasUpper && hasLower;
}

bool UserRegistration::isValidName(const string& name) const {
    if (name.empty() || name.length() > 50) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool UserRegistration::isValidPhone(const string& phone) const {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool UserRegistration::isUsernameTaken(const string& username) const {
    return users.find(username) != users.end();
}

bool UserRegistration::registerUser() {
    Customer newCustomer;
    string input;
    
    cin.ignore();
    cout << "Enter name: ";
    getline(cin, input);
    if (!isValidName(input)) {
        cout << "Invalid name! Use alphabets only.\n";
        return false;
    }
    newCustomer.name = input;

    cout << "Enter phone number (10 digits): ";
    getline(cin, input);
    if (!isValidPhone(input)) {
        cout << "Invalid phone number!\n";
        return false;
    }
    newCustomer.phno = input;

    cout << "Enter bank account number: ";
    getline(cin, input);
    newCustomer.setBankAccountNo(input);

    cout << "Create username: ";
    getline(cin, input);
    if (isUsernameTaken(input)) {
        cout << "Username already taken!\n";
        return false;
    }
    newCustomer.username = input;

    cout << "Create password (min 8 chars, 1 uppercase, 1 lowercase): ";
    getline(cin, input);
    if (!isValidPassword(input)) {
        cout << "Invalid password format.\n";
        return false;
    }

    newCustomer.setPassword(input);
    users[newCustomer.username] = newCustomer;
    cout << "Registration successful!\n";
    saveToFile();
    return true;
}

bool UserRegistration::loginUser() {
    string username, password;
    cin.ignore();
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    auto it = users.find(username);
    if (it == users.end()) {
        cout << "User not found!\n";
        return false;
    }

    Customer& customer = it->second;
    if (customer.isAccountLocked()) {
        cout << "Account locked! Try after 24 hours.\n";
        return false;
    }

    if (customer.verifyPassword(password)) {
        customer.resetFailedAttempts();
        cout << "Login successful! Welcome, " << customer.name << "!\n";
        saveToFile();
        return true;
    } else {
        customer.incrementFailedAttempts();
        cout << "Invalid password! Attempts left: " << (3 - customer.failedAttempts) << "\n";
        if (customer.isAccountLocked())
            cout << "Account locked for 24 hours!\n";
        saveToFile();
        return false;
    }
}

void UserRegistration::saveToFile() const {
    ofstream file("users.dat");
    if (!file.is_open()) {
        cout << "Error saving data!\n";
        return;
    }
    
    // Write number of users on first line
    file << users.size() << "\n";
    
    // Write each user with proper line formatting
    for (const auto& pair : users) {
        file << pair.second;
    }
    file.close();
}

void UserRegistration::loadFromFile() {
    ifstream file("users.dat");
    if (!file.is_open()) {
        cout << "No previous user data found.\n";
        return;
    }
    
    string sizeStr;
    if (!getline(file, sizeStr)) {
        cout << "Error reading user count.\n";
        return;
    }
    
    int userCount = stoi(sizeStr);
    for (int i = 0; i < userCount; i++) {
        Customer cust;
        file >> cust;
        if (file.fail()) break;
        users[cust.username] = cust;
    }
    file.close();
    cout << "Loaded " << userCount << " user(s) from file.\n";
}
