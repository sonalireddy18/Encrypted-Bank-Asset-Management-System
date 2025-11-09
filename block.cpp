#include "block.h"
#include <sstream>
#include <ctime>
#include <iostream>

using namespace std;

// Simple DJB2 hash function for strings
static string hashString(const string& input) {
    unsigned int hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
    }
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

// Calculate the hash representing the block
string Block::calculateHash() const {
    stringstream ss;
    ss << index << previousHash << timestamp;
    for (const auto& tx : transactions) {
        ss << tx.fromAccount << tx.toAccount << tx.amount << tx.timestamp;
    }
    return hashString(ss.str());
}

Block::Block(int idx, const string& prevHash)
    : index(idx), previousHash(prevHash), timestamp(time(nullptr)) {
    hash = calculateHash();
}

void Block::addTransaction(const string& fromAcc, const string& toAcc, double amount) {
    TransactionRecord tx{fromAcc, toAcc, amount, time(nullptr)};
    // Now using operator overloading instead of direct push
    *this += tx;
    // Update hash after adding transaction
    hash = calculateHash();
}

// Operator overloading for adding transactions
Block& Block::operator+=(const TransactionRecord& tx) {
    transactions.push_back(tx);
    return *this;
}

string Block::getHash() const { return hash; }
string Block::getPreviousHash() const { return previousHash; }
int Block::getIndex() const { return index; }

void Block::printBlock() const {
    cout << "Block #" << index << "\n";
    cout << "Previous Hash: " << previousHash << "\n";
    cout << "Hash: " << hash << "\n";
    cout << "Timestamp: " << ctime(&timestamp);
    cout << "Transactions:\n";
    for (const auto& tx : transactions) {
        cout << "  From: " << tx.fromAccount << ", To: " << tx.toAccount
             << ", Amount: " << tx.amount << ", Time: " << ctime(&tx.timestamp);
    }
    cout << "--------------------------------\n";
}

// Blockchain class maintaining chain of blocks
Blockchain::Blockchain() {
    // Create genesis block
    chain.push_back(Block(0, "0"));
}

Block& Blockchain::getLatestBlock() {
    return chain.back();
}

void Blockchain::addTransaction(const string& fromAcc, const string& toAcc, double amount) {
    // Add transaction in new block: For simplicity, new block per transaction
    Block newBlock(chain.size(), getLatestBlock().getHash());
    newBlock.addTransaction(fromAcc, toAcc, amount);
    chain.push_back(newBlock);
}

void Blockchain::printChain() const {
    cout << "Blockchain contents:\n";
    for (const auto& block : chain) {
        block.printBlock();
    }
}

