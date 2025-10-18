#include<iostream>
#include<string>
#include<vector>
#include <climits>
#include <algorithm>
#include "accounts.h"
#include "signup.h"

int main(){
    UserRegistration registeration;
    int ch;
    cout<< "================================"<<endl;
    cout<< "Welcome to bams"<<endl;
    cout<< "================================"<<endl;
    cout<< "1. Register\n2. login\n3. exit"<<endl;
    cout<< "enter your choice: ";
    cin>>ch;
    switch(ch){
        case 1:{
            
        }
        case 2:{

        }
        case 3:{
          return 0;

        }
        default: {
            cout<<"invalid entry try again."<<endl;
        }
    }
}


