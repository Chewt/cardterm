#include "blackjack.h"
#include "crazy8.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv)
{
    float money;

    // Allow user to specify starting money
    if (argc == 2)
        money = strtol(argv[1], NULL, 10);

    // Default to 2000
    money = (money) ? money : 2000;

    srand(time(NULL));
    CLEAR_SCREEN();

    //money = blackjack(money);
    money = crazy8(money);

    printf("Your winnings are $%.2f!\n", money);

    return 0;
}
