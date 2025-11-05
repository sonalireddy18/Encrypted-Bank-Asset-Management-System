#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <ctime>

struct TransactionRecord {
    std::string fromAccount;
    std::string toAccount;
    double amount;
    std::time_t timestamp;
};

class Block {
public:
    Block(int idx, const std::string& prevHash);

    void addTransaction(const std::string& fromAcc, const std::string& toAcc, double amount);

    std::string getHash() const;
    std::string getPreviousHash() const;
    int getIndex() const;

    void printBlock() const;

private:
    int index;
    std::string previousHash;
    std::string hash;
    std::time_t timestamp;
    std::vector<TransactionRecord> transactions;

    std::string calculateHash() const;
};

class Blockchain {
public:
    Blockchain();

    void addTransaction(const std::string& fromAcc, const std::string& toAcc, double amount);
    void printChain() const;

private:
    std::vector<Block> chain;

    Block& getLatestBlock();
};

#endif // BLOCK_H
