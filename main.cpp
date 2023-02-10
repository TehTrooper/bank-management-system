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


#include "include/Customer.h"
#include "include/bank-functions.h"
#include "include/other-functions.h"
/*
#include "Customer.h"
#include "bank-functions.h"
#include "other-functions.h"
*/
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

    if (file_username == username and file_password == password) {
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
            break;

        case 3:
            if ( !user.TransferFunds(std::string("3929996450"), std::string("4510791801"), 1)) {

                std::cout << "Error while transfering funds! \n";

            }
            break;

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