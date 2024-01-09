#include "blackjack.h"
#include "cards.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    /*
    Deck deck = new_deck();
    shuffle(&deck);
    printf("\e[2J\e[H\n");
    int i;
    for (i = 0; i < 5; ++i)
    {
        print_card_pretty(deck.cards[i]);
    }
    printf("\e[5B\r");
    return 0;
    */
    /* MAIN PROGRAM */

    // Seed rand()
    srand(time(NULL));
    CLEAR_SCREEN();

    float money = 2000.0;

    money = blackjack(money);

    printf("Your winnings are $%.2f!\n", money);

    return 0;
}
