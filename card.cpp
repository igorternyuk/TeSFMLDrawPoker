#include "card.h"
#include <algorithm>

poker::Card::Card(Rank rank, Suit suit):
    rank_{rank}, suit_{suit}
{}

int poker::Card::getValue() const noexcept
{
    return static_cast<int>(rank_);
}

std::string poker::Card::toString(const bool isCompressed) const noexcept
{
    return isCompressed ? rankToString() + suitToString() : rankToString(false) +
                   std::string(" of ") + suitToString(false);

}

std::string poker::Card::rankToString(const bool isCompressed) const noexcept
{
    std::string res;
    switch(rank_)
    {
        case Rank::JACK:
            res = isCompressed ? "J" : "jack";
            break;
        case Rank::QUEEN:
            res = isCompressed ? "Q" : "Queen";
            break;
        case Rank::KING:
            res = isCompressed ? "K" : "King";
            break;
        case Rank::ACE:
            res = isCompressed ? "A" : "Ace";
            break;
        default:
            res = std::to_string(getValue());
            break;
    }
    return res;
}

std::string poker::Card::suitToString(const bool isCompressed) const noexcept
{
    std::string res;
    switch(suit_)
    {
        case Suit::CLUBS:
            res = isCompressed ? "C" : "clubs";
            break;
        case Suit::DIAMONDS:
            res = isCompressed ? "D" : "diamonds";
            break;
        case Suit::HEARTS:
            res = isCompressed ? "H" : "hearts";
            break;
        case Suit::SPADES:
            res = isCompressed ? "S" : "spades";
            break;
    }
    return res;
}


std::ostream &poker::operator<<(std::ostream &os,
                                     const poker::Card &card)
{
    os << card.toString();
    return os;
}

bool poker::operator>(const poker::Card &first,
                           const poker::Card &second)
{
    return first.getValue() > second.getValue();
}

bool poker::operator<(const poker::Card &first,
                           const poker::Card &second)
{
    return first.getValue() < second.getValue();
}

bool poker::operator==(const poker::Card &first,
                            const poker::Card &second)
{
    return !(first > second) && !(first < second);
}

bool poker::operator!=(const poker::Card &first,
                            const poker::Card &second)
{
    return !(first == second);
}

std::ostringstream &poker::operator<<(std::ostringstream &oss,
                                     const poker::Card &card)
{
    oss << card.toString();
    return oss;
}

poker::Card::Rank poker::convertIntToRank(int rank)
{
    if(rank < Card::LOWEST_CARD_VALUE)
        rank = Card::LOWEST_CARD_VALUE;
    else if(rank > Card::HIGHEST_CARD_VALUE)
        rank = Card::HIGHEST_CARD_VALUE;
    Card::Rank result = (rank == Card::LOWEST_CARD_VALUE) ? Card::Rank::ACE :
                                                            static_cast<Card::Rank>(rank);
    return result;
}
