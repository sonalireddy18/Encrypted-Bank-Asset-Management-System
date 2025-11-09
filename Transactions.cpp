#include "transaction.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

Blockchain Transaction::blockchain;


static void printLine(const string& text) {
    cout << string(text.size(), '-') << "\n";
}

void Transaction::transferByAccountNoUserRegistration(UserRegistration& ur, const string& f, const string& t, double amt) {
    Customer* fc = ur.getCustomerByAccountNo(f);
    Customer* tc = ur.getCustomerByAccountNo(t);

    string title = "TRANSFER DETAILS";
    cout << "\n" << title << "\n";
    printLine(title);

    if (!fc || !tc) {
        cout << "Error: Invalid account number(s).\n";
        printLine(title);
        return;
    }

    if (dynamic_cast<SavingsAccount*>(fc->account)) {
        cout << "Error: Savings accounts cannot transfer.\n";
        printLine(title);
        return;
    }

    if (fc->account->getBal() >= amt) {
        fc->account->setBal(fc->account->getBal() - amt);
        tc->account->setBal(tc->account->getBal() + amt);
        cout << "Transfer Successful!\n";
        cout << "From Account: " << f << "\nTo Account  : " << t
             << "\nAmount      : Rs " << fixed << setprecision(2) << amt << "\n";
        blockchain.addTransaction(f, t, amt);
    } else {
        cout << "Error: Insufficient balance.\n";
    }

    printLine(title);
}

void Transaction::withdrawUserRegistration(UserRegistration& ur, const string& a, double amt) {
    Customer* c = ur.getCustomerByAccountNo(a);

    string title = "WITHDRAWAL DETAILS";
    cout << "\n" << title << "\n";
    printLine(title);

    if (!c) {
        cout << "Error: Account not found.\n";
        printLine(title);
        return;
    }

    if (c->account->getBal() >= amt) {
        c->account->setBal(c->account->getBal() - amt);
        blockchain.addTransaction(a, "", amt);
        cout << "Withdrawal Successful!\n";
        cout << "Account: " << a << "\nAmount : Rs " << fixed << setprecision(2) << amt << "\n";
    } else {
        cout << "Error: Insufficient balance.\n";
    }

    printLine(title);
}

void Transaction::depositUserRegistration(UserRegistration& ur, const string& a, double amt) {
    Customer* c = ur.getCustomerByAccountNo(a);

    string title = "DEPOSIT DETAILS";
    cout << "\n" << title << "\n";
    printLine(title);

    if (!c) {
        cout << "Error: Account not found.\n";
        printLine(title);
        return;
    }

    c->account->setBal(c->account->getBal() + amt);
    blockchain.addTransaction("", a, amt);
    cout << "Deposit Successful!\n";
    cout << "Account: " << a << "\nAmount : Rs " << fixed << setprecision(2) << amt << "\n";

    printLine(title);
}

void Transaction::printTransactionHistory() {
    blockchain.printChain();
}

// Save blockchain to file
void Transaction::saveBlockchainToFile(const string& filename) {
    ofstream ofs(filename, ios::binary);
    if (!ofs) return;

    stringstream ss;
    for (const auto& block : blockchain.chain) {
        ss << "BLOCK," << block.getIndex() << "," << block.getPreviousHash() << "," << block.getHash() << "," << block.getIndex() << "\n";
    
        for (const auto& tx : block.transactions) {
            ss << "TX," << tx.fromAccount << "," << tx.toAccount << "," << tx.amount << "," << tx.timestamp << "\n";
        }
        ss << "END_BLOCK\n";
    }

    string encrypted = Customer::encryptDecrypt(ss.str());
    ofs.write(encrypted.c_str(), encrypted.size());
    ofs.close();
}


void Transaction::loadBlockchainFromFile(const string& filename) {
    ifstream ifs(filename, ios::binary);
    if (!ifs) return;

    string encrypted((istreambuf_iterator<char>(ifs)), {});
    ifs.close();

    string data = Customer::encryptDecrypt(encrypted);
    stringstream ss(data);
    string line;

    blockchain = Blockchain(); // reset chain
    Block* currentBlock = nullptr;

    while (getline(ss, line)) {
        if (line.rfind("BLOCK", 0) == 0) {
            stringstream l(line);
            string tag, idxStr, prevHash, hashStr, dummy;
            getline(l, tag, ',');
            getline(l, idxStr, ',');
            getline(l, prevHash, ',');
            getline(l, hashStr, ',');
            getline(l, dummy, ',');
            int idx = stoi(idxStr);
            blockchain.chain.push_back(Block(idx, prevHash));
            currentBlock = &blockchain.chain.back();
        } 
        else if (line.rfind("TX", 0) == 0 && currentBlock) {
            stringstream l(line);
            string tag, from, to, amtStr, timeStr;
            getline(l, tag, ',');
            getline(l, from, ',');
            getline(l, to, ',');
            getline(l, amtStr, ',');
            getline(l, timeStr, ',');
            TransactionRecord tx{from, to, stod(amtStr), static_cast<time_t>(stoll(timeStr))};
            currentBlock->operator+=(tx);
        }
    }
}
