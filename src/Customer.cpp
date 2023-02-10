#include "../include/Customer.h"



Customer::Customer(std::string personnummer)
    : person_num(personnummer), accounts_path("data\\" + personnummer + "\\accounts\\") {
        
        /* Loads accounts on initiation  */
        LoadAccounts();

    }

Customer::~Customer() {


}



void Customer::LoadAccounts() {

    /* Clears account details, for reuseability */
    accounts.clear();

    /* Lists all entries in accounts_path */
    /* There are only files in this folder, so there is no need to check for folders */
    for (const auto& file : std::filesystem::directory_iterator(accounts_path)) {

        /* account_balance_string is needed to be a string to extract info from text file */
        std::string account_number, account_balance_string;

        std::ifstream account_file (file.path().generic_string());
        std::getline(account_file, account_number);
        std::getline(account_file, account_balance_string);
    

        /* String to integer */
        float account_balance = std::stof(account_balance_string);

        /* Inserts account details to accounts map */
        accounts.insert({account_number, account_balance});

        /* Is this needed? */
        account_file.close();

    }



}

void Customer::CreateAccount() {
    /* Generates a random 10 digit number */ 
    std::string new_account_number = GenerateAccountNumber();
    
    bool run = true;
    
    /* Varible for checking if there is matching account numbers */
    bool same = false;

    /* This will check everyones account numbers */
    while (run) {
        for (const auto& entry : std::filesystem::directory_iterator("data")) {
            
            /* This ignores everything execpt directories  */
            if (!entry.is_directory()) {continue;}
            
            /* Within everyones personal folders account folder */
            /* Goes though every file in account folder aka accounts */
            for (const auto& account_file : std::filesystem::directory_iterator(std::filesystem::path(entry.path().generic_string() + "\\accounts"))) {
                
                /* Opens file in account folder */
                std::ifstream file (account_file.path().generic_string());

                /* Gets account number */
                std::string temp_account_number;
                std::getline(file, temp_account_number);
                file.close();

                /* If new account number matches new account number */
                /* If accout number already exists, a new number is assagined, and restart from the begining (break) */
                if (temp_account_number == new_account_number) {
                    std::cout << "Account Number Exists \n";
                    std::cout << temp_account_number << "\n" << new_account_number << "\n";
                    new_account_number = GenerateAccountNumber();
                    same = true;
                    break;

                }
            }

            /* Follow up from number match, so if number matches, it restarts from the begining */
            if (same) {
                same = false;
                break;
            }
        }

        /* If there is no matches, then the new account is unique */
        if (!same) {
            run = false;
        }
        
    }

    /* Creates new files and adds account number and balance*/
    std::ofstream file ("data\\" + person_num + "\\accounts\\" + new_account_number + ".txt");

    file << new_account_number << "\n" << "0\n";

    file.close();


}

void Customer::ListAllAccounts() {
    LoadAccounts();

    for (auto entry : accounts) {

        std::cout << entry.first << " : " << entry.second << "\n";

    }

}

bool Customer::IsYourAccount(std::string account_number) {
    LoadAccounts();

    /* Checks if the account number given can be found with your accounts */
    for (auto entry : accounts) {
        if (entry.first == account_number) { return true; }
    }

    return false;
}

std::string Customer::FindAccount(std::string account_number, bool your_folder = false) {
    if ( !your_folder ) {
        for (const auto& entry : std::filesystem::directory_iterator("data")) {
                
            /* This ignores everything execpt directories  */
            if (!entry.is_directory()) {continue;}
                
            /* Within everyones personal folders account folder */
            /* Goes though every file in account folder aka accounts */
            for (const auto& account_file : std::filesystem::directory_iterator(std::filesystem::path(entry.path().generic_string() + "\\accounts"))) {
                    
                /* Opens file in account folder */
                std::ifstream file (account_file.path().generic_string());

                /* Gets account number */
                std::string temp_account_number;
                std::getline(file, temp_account_number);
                file.close();


                if (temp_account_number == account_number) {
                    return account_file.path().generic_string();

                }
            }
        }
    } else {
        /* If your_folder is true */
        /* This is then you want to search your own folder for accounts and not everyones */
        for (const auto& account_file : std::filesystem::directory_iterator(accounts_path)) {
                    
            /* Opens file in account folder */
            std::ifstream file (account_file.path().generic_string());

            /* Gets account number */
            std::string temp_account_number;
            std::getline(file, temp_account_number);
            file.close();


            if (temp_account_number == account_number) {
                return account_file.path().generic_string();

            }
        }
    }

    return std::string("None");

}

bool Customer::TransferFunds(std::string from, std::string to, float amount) {

    std::string you = FindAccount(from, true);
    std::string other = FindAccount(to);


    /* Checks if your account exists */
    if ( you == std::string("None") ) { return false; }
    if ( other == std::string("None") ) { return false; }


    return ChangeBalance(you, other, amount);;

}


