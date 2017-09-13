#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace poker
{
    class Card
    {
    public:
        enum class Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
                          TEN, JACK, QUEEN, KING, ACE };
        enum class Suit { CLUBS, HEARTS, SPADES, DIAMONDS };
        enum
        {
            NUM_SUITS = 4,
            NUM_RANKS = 13,
            LOWEST_CARD_VALUE = 2,
            HIGHEST_CARD_VALUE = 14
        };
        explicit Card(Rank rank = Rank::ACE, Suit suit = Suit::CLUBS);
        inline auto getRank() const noexcept { return rank_; }
        inline auto getSuit() const noexcept { return suit_; }
        inline auto isRank(Rank rank) const noexcept { return rank_ == rank; }
        inline auto isSuit(Suit suit) const noexcept { return suit_ == suit;}
        int getValue() const noexcept;
        std::string toString(const bool isCompressed = true) const noexcept;
        std::string rankToString(const bool isCompressed = true) const noexcept;
        std::string suitToString(const bool isCompressed = true) const noexcept;
    private:
        Rank rank_;
        Suit suit_;
    };

    bool operator==(const Card &first, const Card &second);
    bool operator!=(const Card &first, const Card &second);
    bool operator>(const Card&first, const Card &second);
    bool operator<(const Card&first, const Card &second);
    std::ostream& operator<<(std::ostream& os, const Card &card);
    std::ostringstream& operator<<(std::ostringstream& oss, const Card &card);
    Card::Rank convertIntToRank(int rank);
}

