#include "cards.h"
#include <stdio.h>
#include <stdlib.h>

const char* suit_name(enum suits s)
{
    switch (s)
    {
        case HEARTS:
            return "Hearts";
            break;
        case DIAMONDS:
            return "Diamonds";
            break;
        case CLUBS:
            return "Clubs";
            break;
        case SPADES:
            return "Spades";
            break;
        default:
            return "Unknown suit";
    }

}

const char* rank_name(enum ranks r)
{
    switch (r)
    {
        case ACE:
            return "Ace";
            break;
        case TWO:
            return "Two";
            break;
        case THREE:
            return "Three";
            break;
        case FOUR:
            return "Four";
            break;
        case FIVE:
            return "Five";
            break;
        case SIX:
            return "Six";
            break;
        case SEVEN:
            return "Seven";
            break;
        case EIGHT:
            return "Eight";
            break;
        case NINE:
            return "Nine";
            break;
        case TEN:
            return "Ten";
            break;
        case JACK:
            return "Jack";
            break;
        case QUEEN:
            return "Queen";
            break;
        case KING:
            return "King";
            break;
        default:
            return "Unknown rank";
    }

}

void swap_cards(Card* a, Card* b)
{
    Card temp = *a;
    *a = *b;
    *b = temp;
}

void print_card_text(Card card)
{
    printf("%s of %s", rank_name(card.rank), suit_name(card.suit));
}

void print_card(Card card)
{
    if (card.flipped)
    {
        printf("╔═════╗\e[B\e[7D"
               "║ ╝ ╚ ║\e[B\e[7D"
               "║ ═║═ ║\e[B\e[7D"
               "║ ╗ ╔ ║\e[B\e[7D"
               "╚═════╝\e[4A");
        return;
    }
    char rank = ' ';
    char* suit;
    switch (card.rank)
    {
        case ACE:
            rank = 'A';
            break;
        case JACK:
            rank = 'J';
            break;
        case QUEEN:
            rank = 'Q';
            break;
        case KING:
            rank = 'K';
            break;
        default:
            rank = '1' + card.rank;
    }
    switch (card.suit)
    {
        case SPADES:
            //suit = "SPD";
            suit = " ♠ ";
            break;
        case HEARTS:
            //suit = "HRT";
            suit = " \e[31;1m♥\e[0m ";
            break;
        case CLUBS:
            //suit = "CLB";
            suit = " ♣ ";
            break;
        case DIAMONDS:
            //suit = "DMD";
            suit = " \e[31;1m♦\e[0m ";
            break;
        default:
            suit = " ? ";
            break;
    }
    printf("╔═════╗\e[B\e[7D"
           "║%c%c   ║\e[B\e[7D"
           "║ %s ║\e[B\e[7D"
           "║   %c%c║\e[B\e[7D"
           "╚═════╝\e[4A",
           (card.rank == TEN) ? '1' : rank,
           (card.rank == TEN) ? '0' : ' ',
           suit,
           (card.rank == TEN) ? '1' : ' ',
           (card.rank == TEN) ? '0' : rank);
}

void print_hand(Card* hand, int num_cards)
{
    int i;
    for (i = 0; i < num_cards - 1; ++i)
    {
        print_card(hand[i]);
    }
    print_card(hand[i]);
    printf("\e[5B\r");
    fflush(stdout);
}

Deck new_deck()
{
    Deck deck;
    deck.numCards = 52;
    int i, j, k;
    k = 0;
    for (i = SPADES; i <= HEARTS; ++i)
    {
        for (j = ACE; j <= KING; ++j)
        {
            deck.cards[k].suit = i;
            deck.cards[k].rank = j;
            deck.cards[k].flipped = 0;
            k++;
        }
    }
    return deck;
}

void shuffle(Deck* deck)
{
    int i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < deck->numCards; ++j)
        {
            int idx1 = rand() % deck->numCards;
            int idx2 = rand() % deck->numCards;
            swap_cards(deck->cards + idx1, deck->cards + idx2);
        }
    }
}

Card deal_card(Deck* deck)
{
    Card c = deck->cards[deck->numCards - 1];
    deck->numCards--;
    return c;
}
