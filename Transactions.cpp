#include "Transactions.h"
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

    if (amt <= 0) {
        cout << "Error: Invalid amount. Please enter a positive value and try again.\n";
        printLine(title);
        return;
    }

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
        cout << "From Account: " << f << "\nTo Account : " << t
             << "\nAmount : Rs " << fixed << setprecision(2) << amt << "\n";
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

    // EXCEPTION HANDLING: Negative/Zero check
    if (amt <= 0) {
        cout << "Error: Invalid amount. Please enter a positive value and try again.\n";
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

    // EXCEPTION HANDLING: Negative/Zero check
    if (amt <= 0) {
        cout << "Error: Invalid amount. Please enter a positive value and try again.\n";
        printLine(title);
        return;
    }

    c->account->setBal(c->account->getBal() + amt);
    blockchain.addTransaction("", a, amt);
    cout << "Deposit Successful!\n";
    cout << "Account: " << a << "\nAmount : Rs " << fixed << setprecision(2) << amt << "\n";
    printLine(title);
}

void Transaction::printTransactionHistory(const string& accNo) {
    //Display transaction history for a specific account
    string title = "TRANSACTION HISTORY FOR ACCOUNT " + accNo;
    cout << "\n" << title << "\n";
    printLine(title);

    bool found = false;

    //Iterate through all blocks and transactions
    for (const auto& block : blockchain.chain) {
        for (const auto& tx : block.transactions) {
            if (tx.fromAccount == accNo || tx.toAccount == accNo) {
                found = true;
                cout << "From: " << (tx.fromAccount.empty() ? "(Deposit)" : tx.fromAccount)
                     << " | To: " << (tx.toAccount.empty() ? "(Withdraw)" : tx.toAccount)
                     << " | Amount: Rs " << fixed << setprecision(2) << tx.amount
                     << " | Time: " << ctime(&tx.timestamp);
            }
        }
    }

    if (!found) {
        cout << "No transactions found for this account.\n";
    }

    printLine(title);
}

// Full Blockchain Ledger View
void Transaction::printFullBlockchain() {
    //Display entire blockchain for debugging or admin view
    string title = "FULL BLOCKCHAIN LEDGER";
    cout << "\n" << title << "\n";
    cout << string(title.size(), '-') << "\n";

    if (blockchain.chain.empty()) {
        cout << "Blockchain is empty.\n";
        return;
    }

    //Loop through each block and print details
    for (const auto& block : blockchain.chain) {
        string header = "Block #" + to_string(block.getIndex());
        cout << "\n" << header << "\n";
        cout << string(header.size(), '-') << "\n";

        cout << "Previous Hash : " << block.getPreviousHash() << "\n";
        cout << "Current Hash  : " << block.getHash() << "\n";
        cout << "Transactions  :\n";

        //Print all transactions inside the block
        if (block.transactions.empty()) {
            cout << "   (No transactions in this block)\n";
        } else {
            for (const auto& tx : block.transactions) {
                cout << "   From: " << (tx.fromAccount.empty() ? "(Deposit)" : tx.fromAccount)
                     << " | To: " << (tx.toAccount.empty() ? "(Withdraw)" : tx.toAccount)
                     << " | Amount: Rs " << fixed << setprecision(2) << tx.amount
                     << " | Time: " << ctime(&tx.timestamp);
            }
        }
        cout << string(header.size(), '-') << "\n";
    }

    cout << string(title.size(), '-') << "\n";
}

// Save blockchain to file
void Transaction::saveBlockchainToFile(const string& filename) {
    //Open file for binary writing
    ofstream ofs(filename, ios::binary);
    if (!ofs) return;

    stringstream ss;

    //Write each block and transaction to buffer
    for (const auto& block : blockchain.chain) {
        ss << "BLOCK," << block.getIndex() << "," << block.getPreviousHash() << "," << block.getHash() << "," << block.getIndex() << "\n";
    
        for (const auto& tx : block.transactions) {
            ss << "TX," << tx.fromAccount << "," << tx.toAccount << "," << tx.amount << "," << tx.timestamp << "\n";
        }
        ss << "END_BLOCK\n";
    }

    //Encrypt before saving for basic security
    string encrypted = Customer::encryptDecrypt(ss.str());
    ofs.write(encrypted.c_str(), encrypted.size());
    ofs.close();
}

void Transaction::loadBlockchainFromFile(const string& filename) {
    //Open file for reading
    ifstream ifs(filename, ios::binary);
    if (!ifs) return;

    string encrypted((istreambuf_iterator<char>(ifs)), {});
    ifs.close();

    //Decrypt the data
    string data = Customer::encryptDecrypt(encrypted);
    stringstream ss(data);
    string line;

    //Reset blockchain before loading new data
    blockchain = Blockchain();
    Block* currentBlock = nullptr;

    //Parse file line by line
    while (getline(ss, line)) {
        if (line.rfind("BLOCK", 0) == 0) {
            //Read block metadata
            stringstream l(line);
            string tag, idxStr, prevHash, hashStr, dummy;
            getline(l, tag, ',');
            getline(l, idxStr, ',');
            getline(l, prevHash, ',');
            getline(l, hashStr, ',');
            getline(l, dummy, ',');
            int idx = stoi(idxStr);

            //Create block and add to chain
            blockchain.chain.push_back(Block(idx, prevHash));
            currentBlock = &blockchain.chain.back();
        } 
        else if (line.rfind("TX", 0) == 0 && currentBlock) {
            //Read transaction line
            stringstream l(line);
            string tag, from, to, amtStr, timeStr;
            getline(l, tag, ',');
            getline(l, from, ',');
            getline(l, to, ',');
            getline(l, amtStr, ',');
            getline(l, timeStr, ',');

            //Convert fields into transaction structure
            TransactionRecord tx{from, to, stod(amtStr), static_cast<time_t>(stoll(timeStr))};

            //Add transaction to block
            currentBlock->operator+=(tx);
        }
    }
}
