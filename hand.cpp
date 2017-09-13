#include "hand.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <iostream>

poker::Hand::Hand()
{
    cards_.reserve(NUM_CARDS_MAX);
    value_.combination = CombinationType::NO_HAND;
}

void poker::Hand::changeCard(poker::Card card, int pos)
{
    cards_.at(pos) = card;
    std::sort(cards_.begin(), cards_.end());
    isValueChanged_ = true;
}

poker::Hand::Value poker::Hand::getValue() const
{
    if(isValueChanged_)
    {
        evaluate();
        isValueChanged_ = false;
    }
    //Если карты на руке не менялись то возвращаем кешированное значение
    return value_;
}

void poker::Hand::addCard(const poker::Card &card)
{
    if(cards_.size() < NUM_CARDS_MAX)
    {
        cards_.push_back(card);
        if(cards_.size() == NUM_CARDS_MAX)
        {
            std::sort(cards_.begin(), cards_.end());
            isValueChanged_ = true;
        }
    }
}

void poker::Hand::clear()
{
    cards_.clear();
    value_.combination = CombinationType::NO_HAND;
    value_.pattern.clear();
    isValueChanged_ = true;
}

void poker::Hand::setLowAceAtFirstPlace()
{
    Card tmp = cards_[cards_.size() - 1];
    for(int i{int(cards_.size() - 1)}; i > 0; --i)
        cards_[i] = cards_[i - 1];
    cards_[0] = tmp;
}

std::string poker::Hand::toString() const
{
    std::ostringstream oss;
    for(const auto &card: cards_)
        oss << card << "-";
    switch(getValue().combination)
    {
    case NO_HAND:
        oss << "No hand";
        break;
    case HIGH_CARD:
        oss << "High card";
        break;
    case PAIR:
        oss << "Pair";
        break;
    case TWO_PAIR:
        oss << "Two pairs";
        break;
    case THREE_OF_A_KIND:
        oss << "Triple";
        break;
    case STRAIGHT_WITH_LOW_ACE:
        oss << "Straight with low ace";
        break;
    case STRAIGHT:
        oss << "Straight";
        break;
    case FLUSH:
        oss << "Flush";
        break;
    case FULL_HAUS:
        oss << "Full house";
        break;
    case FOUR_OF_A_KIND:
        oss << "Four of a kind";
        break;
    case STRAIGHT_FLUSH_WITH_LOW_ACE:
        oss << "Straight-flush with low ace";
        break;
    case STRAIGHT_FLUSH:
        oss << "Straight flush";
        break;
    case ROYAL_FLASH:
        oss << "Royal flush";
        break;
    default:
        throw std::runtime_error("¡No puede ser!");
    }
    return oss.str();
}

int poker::Hand::findIndexByValue(int val) const
{
    for(auto i = 0u; i < cards_.size(); ++i)
    {
        if(cards_[i].getValue() == val)
            return i;
    }
    return -1;
}

void poker::Hand::evaluate() const
{
    Value value;

    if(cards_.size() < NUM_CARDS_MAX)
    {
        value.combination = CombinationType::NO_HAND;
    }
    else
    {
        std::map<int, int> occurRank;
        for(int r{Card::LOWEST_CARD_VALUE}; r <= Card::HIGHEST_CARD_VALUE; ++r)
            occurRank[r] = 0;

        std::map<Card::Suit, int> occurSuit;
        for(int s{0}; s < Card::NUM_SUITS; ++s)
            occurSuit[static_cast<Card::Suit>(s)] = 0;

        for(const auto &card: cards_)
        {
            ++occurRank[card.getValue()];
            ++occurSuit[card.getSuit()];
        }

        bool isFlush{false};
        for(auto it = occurSuit.cbegin(); it != occurSuit.end(); ++it)
        {
            if(it->second == NUM_CARDS_MAX)
            {
                isFlush = true;
                break;
            }
        }

        int numPairs{0}, numTrips{0}, numFours{0}, numInARow{0},
            maxInARow{0}, maxValInARow{0};
        for(auto it = occurRank.cbegin(); it != occurRank.cend(); ++it)
        {
            switch (it->second)
            {
                case 0:
                    numInARow = 0;
                    break;
                case 1:
                    ++numInARow;
                    if(it->first > maxValInARow)
                        maxValInARow = it->first;
                    if(numInARow > maxInARow)
                        maxInARow = numInARow;
                    value.pattern.emplace_back(it->first,it->second);
                    break;
                case 2:
                    ++numPairs;
                    value.pattern.emplace_back(it->first,it->second);
                    break;
                case 3:
                    ++numTrips;
                    value.pattern.emplace_back(it->first,it->second);
                    break;
                case 4:
                    ++numFours;
                    value.pattern.emplace_back(it->first,it->second);
                    break;
                default:
                    break;
            }
        }

        if(maxInARow == NUM_CARDS_MAX - 1 &&
           maxValInARow == static_cast<int>(Card::Rank::FIVE))
        {
            value.combination = isFlush ?
                        CombinationType::STRAIGHT_FLUSH_WITH_LOW_ACE :
                        CombinationType::STRAIGHT_WITH_LOW_ACE;
        }
        else if(maxInARow == NUM_CARDS_MAX)
        {
            bool isAceMax = maxValInARow == Card::HIGHEST_CARD_VALUE;
            if(isFlush)
                value.combination = isAceMax ? CombinationType::ROYAL_FLASH :
                                               CombinationType::STRAIGHT_FLUSH;
            else
                value.combination = CombinationType::STRAIGHT;
        }
        else if(numFours > 0)
            value.combination = CombinationType::FOUR_OF_A_KIND;
        else if(numTrips > 0)
            value.combination = numPairs > 0 ? CombinationType::FULL_HAUS :
                                               CombinationType::THREE_OF_A_KIND;
        else if(numPairs == 2)
            value.combination = CombinationType::TWO_PAIR;
        else if(numPairs == 1)
            value.combination = CombinationType::PAIR;
        else if(isFlush)
            value.combination = CombinationType::FLUSH;
        else
            value.combination = CombinationType::HIGH_CARD;

        std::sort(value.pattern.begin(), value.pattern.end(),
                  [](const auto &occur1, const auto &occur2)
        {
            return occur1.numberOfAKind == occur2.numberOfAKind ?
                   occur1.cardValue > occur2.cardValue :
                   occur1.numberOfAKind > occur2.numberOfAKind;
        });
    }
    value_ = value;
}

std::ostream& poker::operator<<(std::ostream &os,
                                      const poker::Hand &hand)
{
    os << hand.toString() << std::endl;
    return os;
}

poker::Hand::Occurence::Occurence(int val, int num):
    cardValue(val), numberOfAKind(num)
{}

int poker::compareHandValues(const poker::Hand::Value &val1,
                                  const poker::Hand::Value &val2)
{
    int result{0};
    if(val1.combination > val2.combination)
        result = 1;
    else if(val1.combination < val2.combination)
        result = -1;
    else
    {
        for(auto i = 0u; i < Hand::NUM_CARDS_MAX; ++i)
        {
            if(val1.pattern[i].cardValue == val2.pattern[i].cardValue)
                continue;
            else
            {
                result = val1.pattern[i].cardValue > val2.pattern[i].cardValue ?
                            1 : -1;
                break;
            }
        }
    }
    return result;
}
