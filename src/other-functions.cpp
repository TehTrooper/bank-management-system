#include "../include/other-functions.h"


std::string GenerateAccountNumber() {
    /* Seeding rand */
    std::srand( time(0) );

    std::string random_number;


    /* Generates a number between 0 and 9 and adds it to random_number, it does this 10 times */
    for (int i = 0 ; i < 10 ; i++) {
        random_number += std::to_string(rand() % 10); 
    }


    return random_number;
}