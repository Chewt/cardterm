#include "cards.h"
#include "texas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HOLDEM_MAX_PLAYERS 5

// SCORING
// =======
// One Pair        - 15 
// Two Pair        - 30
// Three of a kind - 45
// Straight        - 60
// Flush           - 75
// Full House      - 90
// Four of a kind  - 105
// Straight Flush  - 120
//
// Add the Highest card's face value to the score for tie breakers
// Jack  - 11
// Queen - 12
// King  - 13
// Ace   - 14

typedef struct
{
    Card hand[2];
    float bet;
} Player;

int texas_score_hand(Card* hand, Card* table, int n_table)
{
    int score = 0;

    // Until I have actual scoring, this will do
    printf("psssst... I don't have a scoring algorithm, help me out? ");
    char resp[256];
    fgets(resp, 255, stdin);
    resp[255] = 0;
    score = strtol(resp, NULL, 10);

    return score;
}

float texas_holdem(float money)
{
    printf("Texas Hold'em!\n");
    while(1)
    {
        Deck deck = new_deck();
        shuffle(&deck);

        // Set players to a maximum of 5.
        // This will probably be settable in the future.
        int num_players = 5;

        Player players[HOLDEM_MAX_PLAYERS];
        Card table[5];


        // Deal cards to players
        int i, j;
        for (i = 0; i < 2; ++i)
        {
            for (j = 0; j < num_players; ++j)
            {
                players[j].hand[i] = deal_card(&deck);
            }
        }

        CLEAR_SCREEN();
        printf("\n\n\n\n\n\n\nYour hand:\n");
        print_hand(players[i].hand, 2);

        // First round of bets
        printf("What will you do? ");
        char resp[256];
        resp[0] = 0;
        get_player_input(resp);
        if (!strcmp(resp, "bet"))
        {
            printf("How much? ");
            resp[0] = 0;
            players[0].bet = get_player_bet(resp);
        }

        // Flop
        table[0] = deal_card(&deck);
        table[1] = deal_card(&deck);
        table[2] = deal_card(&deck);

        CLEAR_SCREEN();
        printf("Flop:\n");
        print_hand(table, 3);
        printf("\nYour hand:\n");
        print_hand(players[i].hand, 2);

        // Second round of bets
        printf("What will you do? ");
        resp[0] = 0;
        get_player_input(resp);
        if (!strcmp(resp, "bet"))
        {
            printf("How much? ");
            resp[0] = 0;
            players[0].bet += get_player_bet(resp);
        }

        // Turn
        table[3] = deal_card(&deck);

        CLEAR_SCREEN();
        printf("Turn:\n");
        print_hand(table, 4);
        printf("\nYour hand:\n");
        print_hand(players[i].hand, 2);

        // Third round of bets
        printf("What will you do? ");
        resp[0] = 0;
        get_player_input(resp);
        if (!strcmp(resp, "bet"))
        {
            printf("How much? ");
            resp[0] = 0;
            players[0].bet += get_player_bet(resp);
        }

        // River
        table[4] = deal_card(&deck);

        CLEAR_SCREEN();
        printf("River:\n");
        print_hand(table, 5);
        printf("\nYour hand:\n");
        print_hand(players[i].hand, 2);

        // Final round of bets
        printf("What will you do? ");
        resp[0] = 0;
        get_player_input(resp);
        if (!strcmp(resp, "bet"))
        {
            printf("How much? ");
            resp[0] = 0;
            players[0].bet += get_player_bet(resp);
        }

        // Showdown
        // TODO
        return money + players[0].bet;


    }
    return money;
}
