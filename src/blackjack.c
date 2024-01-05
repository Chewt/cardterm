#include "cards.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void clean_resp(char* s)
{
    int i;
    while (s[i] != '\n')
    {
        s[i] = tolower(s[i]);
        i++;
    }
    s[i] = 0;
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
            fgets(resp, 255, stdin);
            resp[255] = 0;
            if (!strcmp(resp, "leave\n"))
                return money;
            bet = atof(resp);
            if (bet < 2.f || bet > 500.f)
                printf("Invalid bet!\n");
        } while (bet < 2.f || bet > 500.f);


        Card hand[2][10];
        Card dealer_hand[10];
        int num_hands = 1;
        int hand_size[2] = {0};
        int d_hand_size = 0;

        // Deal the cards out
        hand[0][hand_size[0]++] = deal_card(&deck);
        dealer_hand[d_hand_size++] = deal_card(&deck);
        hand[0][hand_size[0]++] = deal_card(&deck);
        dealer_hand[d_hand_size++] = deal_card(&deck);

        int player_score = score_hand(hand[0], hand_size[0]);
        int dealer_score = score_hand(dealer_hand, d_hand_size);

        // Check for natural scores of 21
        if (player_score == 21 && dealer_score == 21)
        {
            printf("Dealer and Player Naturals! Push!\n");
            continue;
        }
        else if (player_score == 21)
        {
            int i;
            for (i = 0; i < hand_size[0] - 1; ++i)
            {
                print_card_pretty(hand[0][i]);
            }
            print_card_pretty(hand[0][i]);
            printf("\e[5B\r");
            printf("Player Natural! You win $%.2f!\n", 1.5f * bet);
            money += 1.5f * bet;
            continue;
        }
        else if (dealer_score == 21)
        {
            int i;
            for (i = 0; i < hand_size[0] - 1; ++i)
            {
                print_card_pretty(dealer_hand[i]);
            }
            print_card_pretty(dealer_hand[i]);
            printf("\e[5B\r");
            printf("Dealer Natural! You lose\n");
            money += -bet;
            continue;
        }

        if (hand[0][0].rank == hand[0][1].rank)
        {
            printf("\n\nYour hand is:\n");
            int i;
            for (i = 0; i < hand_size[0] - 1; ++i)
            {
                print_card_pretty(hand[0][i]);
            }
            print_card_pretty(hand[0][i]);
            printf("\e[5B\r");
            printf("\nWould you like to split? (yes/no) ");
            fgets(resp, 255, stdin);
            resp[255] = 0;
            clean_resp(resp);
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
        int j;
        for (j = 0; j < num_hands; ++j)
        {
            while (1)
            {
                printf("\e[2J\e[H\n");
                printf("The dealer has a\n");
                print_card_pretty(dealer_hand[0]);
                printf("\e[5B\r");
                printf("\n\nYour hand is:\n");
                int i;
                for (i = 0; i < hand_size[j] - 1; ++i)
                {
                    print_card_pretty(hand[j][i]);
                }
                print_card_pretty(hand[j][i]);
                printf("\e[5B\r");
                if (score_hand(hand[j], hand_size[j]) > 21)
                {
                    printf("\nBust. You lose\n");
                    money += -bet;
                    bust = 1;
                    break;
                }
                printf("\nStand or Hit? ");
                fgets(resp, 255, stdin);
                resp[255] = 0;
                clean_resp(resp);
                if (!strcmp(resp, "hit"))
                {
                    hand[j][(hand_size[j])++] = deal_card(&deck);
                }
                else
                    break;
            }
        }

        if (bust)
            continue;

        // Dealer's turn
        printf("\n\nDealer hand is:\n");
        int i;
        for (i = 0; i < d_hand_size - 1; ++i)
        {
            print_card_pretty(dealer_hand[i]);
        }
        print_card_pretty(dealer_hand[i]);
        printf("\e[5B\r");
        fflush(stdout);
        while (dealer_score < 17)
        {
            sleep(2);
            printf("\e[5A");
            dealer_hand[d_hand_size++] = deal_card(&deck);
            dealer_score = score_hand(dealer_hand, d_hand_size);
            int i;
            for (i = 0; i < d_hand_size; ++i)
            {
                print_card_pretty(dealer_hand[i]);
            }
            printf("\e[5B\r");
            fflush(stdout);
        } 
        dealer_score = score_hand(dealer_hand, d_hand_size);

        // Determine winner
        for (j = 0; j < num_hands; ++j)
        {
            player_score = score_hand(hand[j], hand_size[j]);

            printf("Your score is %d\n", player_score);
            printf("Dealer's score is %d\n", dealer_score);
            if (dealer_score > 21)
            {
                printf("Dealer bust. You win\n");
                money += bet;
            }
            else if (player_score > dealer_score)
            {
                printf("You win!\n");
                money += bet;
            }
            else if (player_score == dealer_score)
            {
                printf("Push!\n");
            }
            else
            {
                printf("You Lose!\n");
                money += -bet;
            }
        }
    }
    return money;
}
