#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <ctime>
using namespace std;

//Structure to hold transaction details
struct TransactionRecord {
    string fromAccount;
    string toAccount;
    double amount;
    time_t timestamp;
};

class Block {
public:
    //Constructor
    Block(int idx, const string& prevHash);

    //Add transaction to block
    void addTransaction(const string& fromAcc, const string& toAcc, double amount);

    string getHash() const;
    string getPreviousHash() const;
    int getIndex() const;

    void printBlock() const;

    //Operator Overloading to add a transaction
    Block& operator+=(const TransactionRecord& tx);

private:
    int index;
    string previousHash;
    string hash;
    time_t timestamp;
    //Vector for multiple transactions
    vector<TransactionRecord> transactions;

    string calculateHash() const;
};

class Blockchain {
public:
    Blockchain();

    void addTransaction(const string& fromAcc, const string& toAcc, double amount);
    void printChain() const;

private:
    //Vector for chain of blocks
    vector<Block> chain;
    Block& getLatestBlock();
};

#endif //BLOCK_H
