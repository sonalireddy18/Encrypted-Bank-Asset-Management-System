# Encrypted Bank Asset Management System 

## 📌 Project Overview
[cite_start]The **Encrypted Bank Asset Management System (BAMS)** is a C++ based banking platform designed to combine essential financial operations with personal asset management and transaction transparency[cite: 13]. [cite_start]The system integrates a **Budgeting Module** to help users track their spendable income and a **blockchain-inspired ledger** to ensure that every financial activity is recorded with unique hashes for data integrity[cite: 16, 18].

## 🎯 Objectives
* [cite_start]**Core Banking Functions:** Support for user registration, deposits, withdrawals, transfers, and tiered account upgrades (Basic, Silver, Gold)[cite: 15].
* [cite_start]**Budgeting & Asset Management:** Efficient tracking of income (salary), management of recurring bills, and calculation of spending summaries to help users plan their finances [cite: 16, 451-454].
* [cite_start]**Basic Security:** Implementation of password hashing (DJB2 algorithm), simple XOR encryption for file storage, and account lockout mechanisms to protect user credentials [cite: 17, 732-745].
* [cite_start]**Transaction Traceability:** A blockchain structure where every financial activity is recorded in linked blocks to maintain transaction integrity[cite: 18, 1560].
* [cite_start]**Persistence:** File-based storage allows user accounts, bill history, and transaction ledgers to persist across multiple sessions[cite: 19].

## 🛠️ System Specifications
* [cite_start]**Programming Language:** C++ (C++11 or later)[cite: 23].
* [cite_start]**Compiler:** GCC/g++ or any standard C++ compiler[cite: 24].
* [cite_start]**Hardware Requirements:** Minimum 2 GB RAM and 100 MB storage space[cite: 29, 30].



## 🏗️ Architecture & OOP Concepts
This project demonstrates key Object-Oriented Programming principles:
1.  [cite_start]**Encapsulation:** Controlled access to user data (passwords, balances) using private methods and public interfaces[cite: 35, 76].
2.  [cite_start]**Abstraction:** Hiding complex transaction and blockchain logic behind simple class methods[cite: 83, 100].
3.  [cite_start]**Inheritance:** Utilizes hierarchical and multilevel inheritance for tiered accounts (Basic/Silver/Gold) and specialized savings variants[cite: 131, 155].
4.  [cite_start]**Polymorphism:** Runtime polymorphism through virtual `display()` functions and dynamic casting for account type identification[cite: 182, 185, 188].
5.  [cite_start]**Operator Overloading:** The `+=` operator is overloaded to intuitively add transaction records to blockchain blocks[cite: 110, 125].
6.  [cite_start]**Templates:** The `Account<T>` base class uses templates to support different data types for account balances[cite: 262, 306].



## 📁 Project Structure
| Module | File(s) | Description |
| :--- | :--- | :--- |
| **Account** | `account.h/cpp` | [cite_start]Manages account tiers and interest rates[cite: 320]. |
| **Signup** | `signup.h/cpp` | [cite_start]Handles registration, authentication, and basic XOR encryption[cite: 357]. |
| **Blockchain** | `block.h/cpp` | [cite_start]Implements the linked-block structure and DJB2 hashing[cite: 386]. |
| **Transaction** | `Transactions.h/cpp` | [cite_start]Processes transfers, deposits, and withdrawal logic[cite: 418]. |
| **Budget** | `budget.h/cpp` | [cite_start]Manages bills, salaries, and spending summaries[cite: 431]. |
| **Main** | `main.cpp` | [cite_start]Central controller for user dashboards and system state[cite: 468]. |

## ⚠️ Security Note
Please note that the encryption (XOR) and hashing (DJB2) methods used in this project are for educational and demonstrative purposes. They provide basic data obfuscation but are not intended for high-security real-world financial applications.

## 🚀 Getting Started
1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/your-username/BAMS-Project.git](https://github.com/your-username/BAMS-Project.git)
    ```
2.  **Compile the Project:**
    ```bash
    g++ main.cpp signup.cpp block.cpp Transactions.cpp Account.cpp budget.cpp -o BAMS
    ```
3.  **Run the Application:**
    ```bash
    ./BAMS
    ```

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Contributors
* [cite_start]**Maddi Sonali Reddy** (CS24B1060) [cite: 2241]
* [cite_start]**Vootada Vennela** (CS24B1030) [cite: 2244]

---
*Developed for the Object Oriented Programming Course (CS2000) under Dr. Mukku Nisanth Kartheek.*
