#include "blackjack.h"
#include "crazy8.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv)
{
    float money = 0;

    // Allow user to specify starting money
    if (argc == 2)
        money = strtol(argv[1], NULL, 10);

    // Default to 2000
    money = (money) ? money : 2000;

    srand(time(NULL));

    char input[256];
    input[0] = 0;
    while (1)
    {
        if (input[0] == 0)
        {
            CLEAR_SCREEN();
            printf("You have $%.2f\n", money);
            printf("Which game would you like to play?\n"
                    "1. Blackjack\n"
                    "2. Crazy 8s\n"
                    "3. Exit\n: ");
            get_player_input(input);
        }

        int exit_program = 0;
        switch(input[0])
        {
            case '1':
                money = blackjack(money);
                break;
            case '2':
                money = crazy8(money);
                break;
            case '3':
                exit_program = 1;
                break;
            default:
                continue;
        }
        if (exit_program)
            break;

        // Ask to replay
        char game = input[0];
        printf("Would you like to play again? (y/n) ");
        input[0] = 0;
        get_player_input(input);
        if (input[0] == 'y')
            input[0] = game;
        else
            input[0] = 0;
    }

    printf("Your winnings are $%.2f!\n", money);

    return 0;
}
