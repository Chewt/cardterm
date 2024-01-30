#include "crazy8.h"
#include "cards.h"
#include "io.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SCREEN_EDGE 79

// Return a card if user input follows scheme: "ranksuit". 
// ex: "10S" for Ten of Spades
Card get_input_card()
{
    char input[256];
    input[0] = 0;
    get_player_input(input);
    int input_len = strlen(input);

    Card c = {-1, -1, 0};
    if (input_len == 2)
    {
        switch (input[0])
        {
            case 'a':
                c.rank = ACE;
                break;
            case '2':
                c.rank = TWO;
                break;
            case '3':
                c.rank = THREE;
                break;
            case '4':
                c.rank = FOUR;
                break;
            case '5':
                c.rank = FIVE;
                break;
            case '6':
                c.rank = SIX;
                break;
            case '7':
                c.rank = SEVEN;
                break;
            case '8':
                c.rank = EIGHT;
                break;
            case '9':
                c.rank = NINE;
                break;
            case 'j':
                c.rank = JACK;
                break;
            case 'q':
                c.rank = QUEEN;
                break;
            case 'k':
                c.rank = KING;
                break;
            default:
                c.rank = -1;
        }
        switch(input[1])
        {
            case 's':
                c.suit = SPADES;
                break;
            case 'd':
                c.suit = DIAMONDS;
                break;
            case 'c':
                c.suit = CLUBS;
                break;
            case 'h':
                c.suit = HEARTS;
                break;
            default:
                c.suit = -1;
                break;
        }
    }
    else if (input_len == 3)
    {
        if ((input[0] == '1') && (input[1] == '0'))
            c.rank = TEN;
        switch(input[2])
        {
            case 's':
                c.suit = SPADES;
                break;
            case 'd':
                c.suit = DIAMONDS;
                break;
            case 'c':
                c.suit = CLUBS;
                break;
            case 'h':
                c.suit = HEARTS;
                break;
            default:
                c.suit = -1;
                break;
        }
    }
    return c;
}

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

/* Flip a card over from the deck to the pile, and draw the new card to the
 * screen.
 */
void turn_over_card(Deck* deck, Deck* pile)
{
    pile->cards[pile->num_cards++] = deal_card(deck);
    pile->cards[pile->num_cards - 1].flipped = 0;
    cur_SAV();
    cur_HOME();
    cur_X(SCREEN_EDGE - (CARD_WIDTH * 2));
    print_card(pile->cards[pile->num_cards - 1]);
    cur_RES();
    fflush(stdout);
}

void print_table(Deck* deck, Deck* pile, Hand* players, const int num_players)
{
    CLEAR_SCREEN();
    cur_HOME();
    int i;
    for (i = num_players - 1; i >= 0; --i)
        cascade_hand(players[i].cards, players[i].num_cards);
    printf("\n");
    cur_SAV();
    cur_HOME();
    cur_X(SCREEN_EDGE - (CARD_WIDTH * 2));
    print_card(pile->cards[pile->num_cards - 1]);
    print_card((Card){SPADES, ACE, 1});
    cur_RES();
    fflush(stdout);
}

/* Checks if any 8s in hand, which are wild cards.
 * return: index of first match, or -1 if no match
 */
int check_for_eights(Hand* hand)
{
    int i;
    for (i = 0; i < hand->num_cards; ++i)
        if (hand->cards[i].rank == EIGHT)
            return i;
    return -1;
}

/* Takes all cards, except for last card in pile, and moves them to deck. Then
 * deck is shuffled. 
 */
void reshuffle(Deck* deck, Deck* pile)
{
    int i;
    // Move cards from pile back into deck, except for topmost card
    for (i = 0; i < pile->num_cards - 1; ++i)
        deck->cards[i] = pile->cards[i];
    deck->num_cards = pile->num_cards - 1;
    pile->cards[0] = pile->cards[pile->num_cards - 1];
    pile->num_cards = 1;
    // shuffle the deck after moving the cards
    shuffle(deck);
}

/* Checks if Card c exists in hand.
 * return: The index of first match, -1 if no match
 */
int check_for_match(Hand* hand, Card c)
{
    int card_played = -1;
    int j;
    for (j = 0; j < hand->num_cards; j++)
    {
        if ((hand->cards[j].suit == c.suit) || (hand->cards[j].rank == c.rank))
        {
            card_played = j;
            break;
        }
    }
    int eight = check_for_eights(hand);
    if ((card_played < 0) && (eight >= 0))
    {
        card_played = eight;

    }
    return card_played;
}

float crazy8(float money)
{
    int i, j;

    const int NUM_PLAYERS = 4;

    Deck deck = new_deck();
    Deck pile;
    pile.num_cards = 0;
    shuffle(&deck);

    // 4 players, all start with no cards
    Hand players[NUM_PLAYERS];
    for (i = 0; i < NUM_PLAYERS; ++i)
        players[i].num_cards = 0;

    // Deal initial cards
    for (i = 0; i < 5; ++i)
    {
        for (j = 0; j < NUM_PLAYERS; ++j)
        {
            players[j].cards[players[j].num_cards++] = deal_card(&deck);
            if (j != 0)
                players[j].cards[i].flipped = 1; 
        }
    }

    // Print "Deck" on right side of the screen
    CLEAR_SCREEN();
    cur_HOME();
    cur_X(SCREEN_EDGE - CARD_WIDTH);
    print_card((Card){SPADES, ACE, 1});

    // Show player their cards, waiting a bit in between to 
    // simulate being dealt the cards
    for (i = 0; i < 5; ++i)
    {
        cur_HOME();
        for (j = NUM_PLAYERS - 1; j >= 0; --j)
        {
            cascade_hand(players[j].cards, i + 1);
            msleep(300);
        }
    }

    // Print flipped card
    turn_over_card(&deck, &pile);

    printf("\n");

    // Game loop
    while (1)
    {
        for (i = 0; i < NUM_PLAYERS; ++i)
        {
            print_table(&deck, &pile, players, NUM_PLAYERS);
            if (i == 0) // Player's Turn 
            {
                // Check if player has any cards that can be played
                int idx = check_for_match(players + i, peek_top(&pile));
                if (idx < 0)
                {
                    cur_Y(-1);
                    printf("You have no playable cards...Drawing\n");
                    players[i].cards[players[i].num_cards++] = deal_card(&deck);
                    if (deck.num_cards == 0)
                        reshuffle(&deck, &pile);
                    i--;
                    msleep(1500);
                    continue;
                }

                printf("\r                    \rSelect Card: ");
                Card c = get_input_card();
                if ((c.rank == -1) || (c.suit == -1))
                {
                    cur_Y(-2);
                    printf("Invalid input\n");
                    i--;
                    continue;
                }
                idx = is_in_hand(players + i, c);
                if (idx < 0)
                {
                    cur_Y(-2);
                    printf("You don't have that card!\n");
                    i--;
                    continue;
                }
                Card top = peek_top(&pile);
                if (   (c.rank == EIGHT) 
                    || (c.rank == top.rank)
                    || (c.suit == top.suit)) 
                {
                  pile.cards[pile.num_cards++] = c;
                  remove_card(players + i, idx);
                }
                else
                {
                    cur_Y(-2);
                    printf("You can't play that card!\n");
                    i--;
                    continue;
                }
            }
            else // Computer's turn
            {
                // Check if any cards match
                int card_played = check_for_match(players + i, peek_top(&pile));
                if (card_played >= 0) // Match found
                {
                  pile.cards[pile.num_cards++] = players[i].cards[card_played];
                  pile.cards[pile.num_cards - 1].flipped = 0;
                  remove_card(players + i, card_played);
                }
                else if (card_played < 0) // No match, draw card
                {
                    players[i].cards[players[i].num_cards++] = deal_card(&deck);
                    players[i].cards[players[i].num_cards - 1].flipped = 1;
                    if (deck.num_cards == 0)
                        reshuffle(&deck, &pile);
                    i--;
                }
                msleep(1000);
            }
            if (players[i].num_cards == 0)
            {
                print_table(&deck, &pile, players, NUM_PLAYERS);
                cur_Y(-1);
                if (i == 0)
                    printf("You win!\n");
                else
                    printf("Player %d wins!\n", NUM_PLAYERS - i);
                return money;
            }
        }
    }
    return money;
}
