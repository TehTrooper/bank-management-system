#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

/* For seeding rand */
/* 
https://stackoverflow.com/questions/7748071/same-random-numbers-every-time-i-run-the-program
*/
#include <ctime>


class Customer {
private:

    /* Social Security Number, acts like username */
    const std::string person_num;

    /* Path to data/{user}/accounts */
    const std::filesystem::path accounts_path;

    /* Unordered Map of of accounts with account number and account balance*/
    std::unordered_map<std::string, float> accounts;

    /* Generates a new unique account number */
    std::string GenerateAccountNumber();

    /* Finds (and returns the file path) account from either your folder or another users folders */
    std::string FindAccount(bool your_folder, std::string account_number);

    /* Checks if the account given is yours */
    bool IsYourAccount(std::string account_number);

    /* Changes balance between accounts without any funds- or account checks */
    int ChangeBalance(std::string from_account_path, std::string to_account_path, float amount);
    

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

Customer::Customer(std::string personnummer)
    : person_num(personnummer), accounts_path("data\\" + personnummer + "\\accounts\\") {
        
        /* Loads accounts on initiation  */
        LoadAccounts();

    }

Customer::~Customer() {


}

std::string Customer::GenerateAccountNumber() {
    /* Seeding rand */
    std::srand( time(0) );

    std::string random_number;


    /* Generates a number between 0 and 9 and adds it to random_number, it does this 10 times */
    for (int i = 0 ; i < 10 ; i++) {
        random_number += std::to_string(rand() % 10); 
    }


    return random_number;
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

std::string Customer::FindAccount(bool your_folder = false, std::string account_number) {
    if ( your_folder ) {
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
}

int Customer::ChangeBalance(std::string from_account_path, std::string to_account_path, float amount) {
    /* Opens from account file to check balance */
    std::ifstream file (from_account_path);
    float balance;
    
    std::string account_number;
    std::string string_balance; 

    /* Gets account balance and number */
    std::getline(file, account_number);
    std::getline(file, string_balance);

    file.close();

    balance = stof(string_balance);

    /* If you can draw from your funds without the balance going under 0 */
    if ( (balance - amount) < 0 ) {

        /* Opens file to change balance */
        std::ofstream file (from_account_path);


        /* Need to add */
        /* Change balance on from and to account */
        /* return 1 if succesful */
        /* return 0 if not */
    }


}

bool Customer::TransferFunds(std::string from, std::string to, float amount) {

    /* Checks if your account exists */
    if ( !IsYourAccount(from) ) { return false; }

    /* Need to add */
    /* Check if other account exists */
    /* Call function to transfer funds */
    /* Return True of transfer is succesfull*/

}

void Register(std::string username, std::string password) {

    /* Checks if data filer exists */
    if (!std::filesystem::is_directory("data")) 
    {
        std::filesystem::create_directory("data");
    }

    /* Creates nessesary directories */
    std::filesystem::create_directory("data\\" + username);
    std::filesystem::create_directory("data\\" + username + "\\accounts");

    /* Creates file and puts in login credential */
    std::ofstream file ("data\\" + username + "\\" + username + ".txt");
    file << username << "\n" << password << "\n";



}

bool Login(std::string username, std::string password) {

    std::string file_username, file_password;

    /* Loads file and gets login credentials */
    std::ifstream file ("data\\" + username + "\\" + username + ".txt");
    std::getline(file, file_username);
    std::getline(file, file_password);

    if (file_username == username && file_password == password) {
        return true;
    }

    return false;

}

/* User interface for accounts */
void Accounts(Customer& user) {

    int choice;

    bool run = true;

    while (run) {

        std::cout << "1. Create Accout \n";
        std::cout << "2. List Accounts \n";
        std::cout << "3. Transfer Balance \n";
        std::cout << "0. Exit \n";

        std::cin >> choice;

        switch (choice)
        {
        case 0:
            run = false;
            break;

        case 1:
            user.CreateAccount();
            break;
        
        case 2:
            user.ListAllAccounts();

        default:
            break;
        }


    }


}

int main() {  

    int run = true;

    /* Create varible outside While loop so it wont go out of scope for later use */
    std::string person_num;

    while (run) {

        int choice;
        
        /* Creates inside here so it will go out of scope after, it will not be used after this */
        std::string password;

        std::cout << "1. Register \n";
        std::cout << "2. Login \n";
        std::cout << "0. Exit \n";

        std::cin >> choice;
        

        switch (choice)
        {
        
        case 0:
            return 1;

        case 1:
            {
                std::cout << "Personnummer: ";
                std::cin >> person_num;

                std::cout << "Password: ";
                std::cin >> password;

                Register(person_num, password);
            }
            break;

        case 2:
            {

                std::cout << "Personnummer: ";
                std::cin >> person_num;

                std::cout << "Password: ";
                std::cin >> password;
                
                if (Login(person_num, password))
                {
                    /* Creates user outside of while loop so it does not go out of scope */
                    run = false;
                }else{

                    std::cout << "Error while logging in! \n";

                }
            }
            break;

        default:
            break;
        }

    }

    /* Loads user, program will not go here if user does not exist or password does not match, so the pasword will not be needed here */
    Customer user(person_num);

    run = true;

    while (run) {

        int choice;

        std::cout << "1. Accounts \n";
        std::cout << "0. Exit \n";

        std::cin >> choice;

        switch (choice)
        {
        case 0:
            run = false;
            break;
        case 1:
            Accounts(user);
            break;
        
        default:
            break;
        }

    }

    std::cout << "Bye";

}