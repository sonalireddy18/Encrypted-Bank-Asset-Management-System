#include "Transaction.h"
#include <iostream>
using namespace std;

Blockchain Transaction::blockchain;

void Transaction::transferByAccountNoUserRegistration(UserRegistration& ur,const string& f,const string& t,double amt){
    Customer* fc = ur.getCustomerByAccountNo(f);
    Customer* tc = ur.getCustomerByAccountNo(t);
    if(dynamic_cast<SavingsAccount*>(fc->account)){
        cout<<"Savings cannot transfer.\n"; return;
    }
    if(fc && tc){
        if(fc->account->getBal()>=amt){
            fc->account->setBal(fc->account->getBal()-amt);
            tc->account->setBal(tc->account->getBal()+amt);
            cout<<"Transfer OK\n";
            blockchain.addTransaction(f,t,amt);
        } else cout<<"Insufficient\n";
    }
}

void Transaction::withdrawUserRegistration(UserRegistration& ur,const string& a,double amt){
    Customer* c = ur.getCustomerByAccountNo(a);
    if(c->account->getBal()>=amt){
        c->account->setBal(c->account->getBal()-amt);
        blockchain.addTransaction(a,"",amt);
        cout<<"Withdraw OK\n";
    } else cout<<"Insufficient\n";
}
void Transaction::depositUserRegistration(UserRegistration& ur,const string& a,double amt){
    Customer* c = ur.getCustomerByAccountNo(a);
    c->account->setBal(c->account->getBal()+amt);
    blockchain.addTransaction("",a,amt);
    cout<<"Deposit OK\n";
}
void Transaction::printTransactionHistory(){ blockchain.printChain(); }
