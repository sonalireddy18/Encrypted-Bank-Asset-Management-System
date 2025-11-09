#include "block.h"
#include <sstream>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

// Local helper (file-scope only)
static void printLine(const string& text) {
    cout << string(text.size(), '-') << "\n";
}

// Simple DJB2 hash
static string hashString(const string& input) {
    unsigned int hash = 5381;
    for (char c : input) hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    stringstream ss; ss << hex << hash;
    return ss.str();
}

string Block::calculateHash() const {
    stringstream ss;
    ss << index << previousHash << timestamp;
    for (const auto& tx : transactions)
        ss << tx.fromAccount << tx.toAccount << tx.amount << tx.timestamp;
    return hashString(ss.str());
}

Block::Block(int idx, const string& prevHash)
    : index(idx), previousHash(prevHash), timestamp(time(nullptr)) {
    hash = calculateHash();
}

void Block::addTransaction(const string& fromAcc, const string& toAcc, double amount) {
    TransactionRecord tx{fromAcc, toAcc, amount, time(nullptr)};
    *this += tx;
    hash = calculateHash();
}

Block& Block::operator+=(const TransactionRecord& tx) {
    transactions.push_back(tx);
    return *this;
}

string Block::getHash() const { return hash; }
string Block::getPreviousHash() const { return previousHash; }
int Block::getIndex() const { return index; }

void Block::printBlock() const {
    string header = "Block #" + to_string(index);
    cout << "\n" << header << "\n";
    printLine(header);

    cout << "Previous Hash : " << previousHash << "\n";
    cout << "Block Hash    : " << hash << "\n";
    cout << "Timestamp     : " << ctime(&timestamp);
    cout << "Transactions  :\n";

    for (const auto& tx : transactions) {
        cout << "   From: " << (tx.fromAccount.empty() ? "(Deposit)" : tx.fromAccount)
             << " | To: " << (tx.toAccount.empty() ? "(Withdraw)" : tx.toAccount)
             << " | Amount: Rs " << fixed << setprecision(2) << tx.amount
             << " | Time: " << ctime(&tx.timestamp);
    }
    printLine(header);
}

Blockchain::Blockchain() { chain.push_back(Block(0, "0")); }

Block& Blockchain::getLatestBlock() { return chain.back(); }

void Blockchain::addTransaction(const string& fromAcc, const string& toAcc, double amount) {
    Block newBlock(chain.size(), getLatestBlock().getHash());
    newBlock.addTransaction(fromAcc, toAcc, amount);
    chain.push_back(newBlock);
}

void Blockchain::printChain() const {
    string title = "BLOCKCHAIN CONTENTS";
    cout << "\n" << title << "\n";
    printLine(title);

    for (const auto& block : chain)
        block.printBlock();

    printLine(title);
}
