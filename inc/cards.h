#ifndef CARDS_H
#define CARDS_H

#define CARD_WIDTH 7
#define CARD_HEIGHT 5

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
    int flipped;
} Card;

typedef struct
{
    Card cards[52];
    int num_cards;
} Deck;

typedef struct
{
    Card cards[52];
    int num_cards;
} Hand;

const char* suit_name(enum suits s);
const char* rank_name(enum ranks r);
void swap_cards(Card* a, Card* b);
void print_card_text(Card card);
void print_card(Card card);
void print_hand(Card* hand, int num_cards);
void cascade_hand(Card* hand, int num_cards);
Deck new_deck();
void shuffle(Deck* deck);
Card deal_card(Deck* deck);
void remove_card(Hand* hand, int idx);
Card peek_top(Deck* deck);
#endif
