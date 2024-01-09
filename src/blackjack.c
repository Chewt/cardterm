#include "cards.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void get_player_input(char* s)
{
    if (s[0] == 0)
        fgets(s, 255, stdin);
    s[255] = '\n';
    int i;
    while (s[i] != '\n')
    {
        s[i] = tolower(s[i]);
        i++;
    }
    s[i] = 0;
}

float get_player_bet(char* s)
{
    float bet = 0.f;
    if (s[0] == 0)
        fgets(s, 255, stdin);
    bet = strtof(s, NULL);
    return bet;
}


int score_hand(Card* hand, int hand_size)
{
    int score = 0;
    int i;
    int aces = 0;
    for (i = 0; i < hand_size; ++i)
    {
        if (hand[i].rank == 0)
        {
            aces += 1;
        }
        else
        {
            int value = hand[i].rank + 1;
            score += (value > 10) ? 10 : value;
        }
    }
    for (i = 0; i < aces; ++i)
    {
        score += ((score + 11) > 21) ? 1 : 11;
    }
    return score;
}

float blackjack(float money)
{
    printf("Blackjack!\n\n");
    while (1)
    {
        Deck deck = new_deck();
        shuffle(&deck);
        int bust = 0;

        // If money drops to zero or below, kick the player out
        if (money <= 0.f)
        {
            printf("You have no more money... leave the table!\n");
            return money;
        }

        // Tell the player how much money they have, and allow them to
        // bet an amount of money between $2 and $500
        float bet = 0.f;
        char resp[256];
        do 
        {
            printf("You have $%.2f\n", money);
            printf("How much would you like to bet, "
                    "between $2 and $500? ");
            resp[0] = 0;
            get_player_input(resp);
            if (!strcmp(resp, "leave"))
                return money;
            bet = get_player_bet(resp);
            if (bet < 2.f || bet > 500.f)
                printf("Invalid bet!\n");
        } while (bet < 2.f || bet > 500.f);
        CLEAR_SCREEN();


        // Deal the cards out
        Card hand[2][10];
        Card dealer_hand[10];
        int num_hands = 1;
        int hand_size[2] = {0};
        int d_hand_size = 0;


        hand[0][hand_size[0]++] = deal_card(&deck);
        dealer_hand[d_hand_size++] = deal_card(&deck);
        hand[0][hand_size[0]++] = deal_card(&deck);
        dealer_hand[d_hand_size++] = deal_card(&deck);

        // SPLIT HAND TESTING
        //hand[0][0] = (Card){SPADES, ACE};
        //hand[0][1] = (Card){CLUBS, ACE};

        int player_score = score_hand(hand[0], hand_size[0]);
        int dealer_score = score_hand(dealer_hand, d_hand_size);

        // Check for natural scores of 21
        if (player_score == 21 && dealer_score == 21)
        {
            printf("Dealer Hand:\n");
            print_hand(dealer_hand, d_hand_size);
            printf("Player Hand:\n");
            print_hand(hand[0], hand_size[0]);
            printf("Dealer and Player Naturals! Push!\n");
            continue;
        }
        else if (player_score == 21)
        {
            print_hand(hand[0], hand_size[0]);
            printf("Player Natural! You win $%.2f!\n", 1.5f * bet);
            money += 1.5f * bet;
            continue;
        }
        else if (dealer_score == 21)
        {
            print_hand(dealer_hand, d_hand_size);
            printf("Dealer Natural! You lose\n");
            money += -bet;
            continue;
        }

        // Check for a possible split hand
        if (hand[0][0].rank == hand[0][1].rank)
        {
            CLEAR_SCREEN();
            printf("\n\nYour hand is:\n");
            print_hand(hand[0], hand_size[0]);
            printf("\nWould you like to split? (yes/no) ");
            resp[0] = 0;
            get_player_input(resp);
            if (!strcmp(resp, "yes"))
            {
                num_hands = 2;
                hand[1][0] = hand[0][1];
                hand[0][1] = deal_card(&deck);
                hand[1][1] = deal_card(&deck);
                hand_size[1] = 2;
            }
        }

        // Let the player choose to hit or stand
        int i;
        for (i = 0; i < num_hands; ++i)
        {
            while (1)
            {
                CLEAR_SCREEN();
                printf("The dealer has a\n");
                print_hand(dealer_hand, 1); // Only show first card

                printf("\n\nYour"); 
                if (num_hands == 2)
                    printf(" %s", (i == 0) ? "first" : "second");
                printf(" hand is:\n");
                print_hand(hand[i], hand_size[i]);

                if (score_hand(hand[i], hand_size[i]) > 21)
                {
                    printf("\nBust. You lose\n");
                    bust++;
                    break;
                }

                printf("\nStand or Hit? ");
                resp[0] = 0;
                get_player_input(resp);
                if (!strcmp(resp, "hit"))
                {
                    hand[i][(hand_size[i])++] = deal_card(&deck);
                }
                else
                    break;
            }
            if (i == 0 && num_hands == 2)
                sleep(2);
        }

        if (bust == num_hands)
        {
            money -= bet * num_hands;
            continue;
        }

        // Dealer's turn
        printf("\n\nDealer hand is:\n");
        print_hand(dealer_hand, d_hand_size);
        while (dealer_score < 17)
        {
            sleep(2);
            printf("\e[5A"); // Draw over old hand
            dealer_hand[d_hand_size++] = deal_card(&deck);
            dealer_score = score_hand(dealer_hand, d_hand_size);
            print_hand(dealer_hand, d_hand_size);
        } 
        dealer_score = score_hand(dealer_hand, d_hand_size);

        // Determine winner
        for (i = 0; i < num_hands; ++i)
        {
            player_score = score_hand(hand[i], hand_size[i]);

            printf("Your");
            if (num_hands == 2)
                printf(" %s", (i == 0) ? "first" : "second");
            printf(" score is %d\n", player_score);
            printf("Dealer's score is %d\n", dealer_score);
            if (player_score > 21)
            {
                printf("Bust! You lose $%.2f!\n", bet);
                money -= bet;
            }
            else if (dealer_score > 21)
            {
                printf("Dealer bust. You win $%.2f!\n", bet);
                money += bet;
            }
            else if (player_score > dealer_score)
            {
                printf("You win $%.2f!\n", bet);
                money += bet;
            }
            else if (player_score == dealer_score)
            {
                printf("Push!\n");
            }
            else
            {
                printf("You Lose $%.2f!\n", bet);
                money += -bet;
            }
        }
    }
    return money;
}
