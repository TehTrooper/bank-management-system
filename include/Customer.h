#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include "other-functions.h"
#include "bank-functions.h"

class Customer {
private:

    /* Social Security Number, acts like username */
    const std::string person_num;

    /* Path to data/{user}/accounts */
    const std::filesystem::path accounts_path;

    /* Unordered Map of of accounts with account number and account balance*/
    std::unordered_map<std::string, float> accounts;


    /* Finds (and returns the file path) account from either your folder or another users folders */
    std::string FindAccount(std::string account_number, bool your_folder);

    /* Checks if the account given is yours */
    /* Can be replaced by FindAccount */
    bool IsYourAccount(std::string account_number);





public:

    Customer(std::string personnummer);
    ~Customer();

    /* Loads or refreshes all accounts from accounts_path to accounts varible (unordered map) */
    void LoadAccounts();

    /* Creates account to accounts_path */
    void CreateAccount();

    /* Types out all accounts */
    void ListAllAccounts();

    /* Transfers funds from one account (the logged in user) to another account */
    /* Also checks if the accounts given exists and if the transfer can happen without and negative balances */
    bool TransferFunds(std::string from, std::string to, float amount);

};