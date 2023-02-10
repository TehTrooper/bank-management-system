#include "../include/bank-functions.h"


int ChangeBalance(std::string from_account_path, std::string to_account_path, float amount) {

    if ( amount < 0 ) { return false; }




    /* Check the giving account */
    /* Opens from account file to check balance */
    std::ifstream from_file (from_account_path);
    float from_balance;
    
    std::string from_account_number;
    std::string from_string_balance; 

    /* Gets account balance and number */
    std::getline(from_file, from_account_number);
    std::getline(from_file, from_string_balance);

    from_balance = stof(from_string_balance);
    if ( ((from_balance - amount ) < 0)) { return false; }

    from_file.close();



    /* Sets up the account that gets the money */
    std::ifstream to_file (to_account_path);
    float to_balance;
    
    std::string to_account_number;
    std::string to_string_balance; 

    /* Gets account balance and number */
    std::getline(to_file, to_account_number);
    std::getline(to_file, to_string_balance);

    to_balance = stof(to_string_balance);


    to_file.close();

    std::ofstream new_from_file (from_account_path);
    float new_balance = from_balance - amount;
    new_from_file << from_account_number << "\n" << std::to_string(new_balance) << "\n";

    new_from_file.close();

    std::ofstream new_to_file (to_account_path);
    new_balance = to_balance + amount;
    new_to_file << to_account_number << "\n" << std::to_string(new_balance) << "\n";

    new_to_file.close();

    return 1;




}


