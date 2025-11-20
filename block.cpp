#include "block.h"
#include <sstream>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

//Helper: prints a line of dashes
static void printLine(const string& text) {
    cout << string(text.size(), '-') << "\n";
}

//Helper: simple DJB2 hash
static string hashString(const string& input) {
    unsigned int hash = 5381;
    for (char c : input) hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    stringstream ss; ss << hex << hash;
    return ss.str();
}

//Calculate hash for block
string Block::calculateHash() const {
    stringstream ss;

    //Add block metadata
    ss << index << previousHash << timestamp;

    //Add all transactions
    for (const auto& tx : transactions)
        ss << tx.fromAccount << tx.toAccount << tx.amount << tx.timestamp;

    //Return hashed value
    return hashString(ss.str());
}

//Block constructor
Block::Block(int idx, const string& prevHash)
    : index(idx), previousHash(prevHash), timestamp(time(nullptr)) {
    
    //Generate hash at creation
    hash = calculateHash();
}

//Add a transaction to block
void Block::addTransaction(const string& fromAcc, const string& toAcc, double amount) {

    //Create transaction record
    TransactionRecord tx{fromAcc, toAcc, amount, time(nullptr)};

    //Append using operator+=
    *this += tx;

    //Recalculate block hash
    hash = calculateHash();
}

//Append transaction record
Block& Block::operator+=(const TransactionRecord& tx) {

    //Store transaction
    transactions.push_back(tx);

    return *this;
}

//Get block hash
string Block::getHash() const { 
    return hash; 
}

//Get previous block hash
string Block::getPreviousHash() const { 
    return previousHash; 
}

//Get block index
int Block::getIndex() const { 
    return index; 
}

//Print full block data
void Block::printBlock() const {

    //Print header
    string header = "Block #" + to_string(index);
    cout << "\n" << header << "\n";
    printLine(header);

    //Print metadata
    cout << "Previous Hash : " << previousHash << "\n";
    cout << "Block Hash    : " << hash << "\n";
    cout << "Timestamp     : " << ctime(&timestamp);
    cout << "Transactions  :\n";

    //Print each transaction
    for (const auto& tx : transactions) {
        cout << "   From: " << (tx.fromAccount.empty() ? "(Deposit)" : tx.fromAccount)
             << " | To: " << (tx.toAccount.empty() ? "(Withdraw)" : tx.toAccount)
             << " | Amount: Rs " << fixed << setprecision(2) << tx.amount
             << " | Time: " << ctime(&tx.timestamp);
    }

    //End block print
    printLine(header);
}

//Blockchain constructor
Blockchain::Blockchain() { 
    
    //Create genesis block
    chain.push_back(Block(0, "0")); 
}

//Get latest block
Block& Blockchain::getLatestBlock() { 
    return chain.back(); 
}

//Add transaction to blockchain
void Blockchain::addTransaction(const string& fromAcc, const string& toAcc, double amount) {

    //Create new block linked to previous one
    Block newBlock(chain.size(), getLatestBlock().getHash());

    //Add transaction into new block
    newBlock.addTransaction(fromAcc, toAcc, amount);

    //Append to chain
    chain.push_back(newBlock);
}

//Print entire blockchain
void Blockchain::printChain() const {

    //Header
    string title = "BLOCKCHAIN CONTENTS";
    cout << "\n" << title << "\n";
    printLine(title);

    //Print every block
    for (const auto& block : chain)
        block.printBlock();

    //Footer
    printLine(title);
}
