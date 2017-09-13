#pragma once
#include "card.h"
#include <vector>
#include <memory>

namespace poker
{
    class Player;
    class Deck
    {
    public:
        explicit Deck();
        inline auto cardsLeft() const { return cards_.size(); }
        void populate();
        void shuffle();
        void deal(Player &player);
        Card top();
    private:
        enum { NUM_CARDS = 52 };
        std::vector<Card> cards_;
    };
}

