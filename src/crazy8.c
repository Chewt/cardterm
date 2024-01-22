#include "crazy8.h"
#include "cards.h"
#include "io.h"
#include <stdio.h>
#include <time.h>

#define SCREEN_EDGE 79

typedef struct
{
    Card hand[52];
    int num_cards;
} C8Player;

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
        return -1;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res);

    return res;
}

void turn_over_card(Deck* deck, Deck* pile)
{
    pile->cards[pile->numCards++] = deal_card(deck);
    pile->cards[pile->numCards - 1].flipped = 0;
    printf("\e[s"); // Save cursor pos
    mov_HOME();
    mov_hoz(SCREEN_EDGE - (CARD_WIDTH * 2));
    print_card(pile->cards[pile->numCards - 1]);
    printf("\e[u"); // Restore cursor pos
}

float crazy8(float money)
{
    int i, j;

    const int NUM_PLAYERS = 4;

    Deck deck = new_deck();
    Deck pile;
    pile.numCards = 0;
    shuffle(&deck);

    // 4 players, all start with no cards
    C8Player players[NUM_PLAYERS];
    for (i = 0; i < NUM_PLAYERS; ++i)
        players[i].num_cards = 0;

    // Deal cards
    for (i = 0; i < 5; ++i)
    {
        for (j = 0; j < NUM_PLAYERS; ++j)
        {
            players[j].hand[players[j].num_cards++] = deal_card(&deck);
            if (j != 0)
                players[j].hand[i].flipped = 1;
        }
    }

    // Print "Deck" on right side of the screen
    CLEAR_SCREEN();
    mov_HOME();
    mov_hoz(SCREEN_EDGE - CARD_WIDTH);
    print_card((Card){SPADES, ACE, 1});

    // Show player their cards, waiting half a second in between to 
    // simulate being dealt the cards
    for (i = 0; i < 5; ++i)
    {
        mov_HOME();
        for (j = NUM_PLAYERS - 1; j >= 0; --j)
        {
            print_hand(players[j].hand, i + 1);
            //cascade_hand(players[j].hand, i + 1);
            msleep(300);
        }
    }

    // Print flipped card
    turn_over_card(&deck, &pile);


    return money;
}
