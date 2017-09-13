#include "deck.h"
#include "player.h"
#include <ctime>
#include <algorithm>

poker::Deck::Deck()
{
    cards_.reserve(NUM_CARDS);
    srand(time(0));
}

void poker::Deck::populate()
{
    cards_.clear();
    for(int s{0}; s < Card::NUM_SUITS; ++s)
    {
        for(int r {Card::LOWEST_CARD_VALUE}; r < Card::HIGHEST_CARD_VALUE; ++r)
        {
            cards_.emplace_back(static_cast<Card::Rank>(r),
                                static_cast<Card::Suit>(s));
        }
    }
}

void poker::Deck::shuffle()
{
    std::random_shuffle(cards_.begin(), cards_.end());
}

void poker::Deck::deal(poker::Player &player)
{
    player.addCard(top());
}

poker::Card poker::Deck::top()
{
    Card card = cards_.back();
    cards_.pop_back();
    return card;
}
