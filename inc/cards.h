#ifndef CARDS_H
#define CARDS_H

// Clear screen macro
#define CLEAR_SCREEN() printf("\e[2J\e[H\n"); 

enum suits
{
    SPADES,
    DIAMONDS,
    CLUBS,
    HEARTS
};

enum ranks
{
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

typedef struct 
{
    enum suits suit;
    enum ranks rank;
} Card;

typedef struct
{
    Card cards[52];
    int numCards;
} Deck;

const char* suit_name(enum suits s);
const char* rank_name(enum ranks r);
void swap_cards(Card* a, Card* b);
void print_card_text(Card card);
void print_card(Card card);
void print_hand(Card* hand, int num_cards);
Deck new_deck();
void shuffle(Deck* deck);
Card deal_card(Deck* deck);
#endif
