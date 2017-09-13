#pragma once

#include "card.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

namespace poker
{
    class Hand
    {
    public:
        enum CombinationType
        {
            NO_HAND,
            HIGH_CARD,
            PAIR,
            TWO_PAIR,
            THREE_OF_A_KIND,
            STRAIGHT_WITH_LOW_ACE,
            STRAIGHT,
            FLUSH,
            FULL_HAUS,
            FOUR_OF_A_KIND,
            STRAIGHT_FLUSH_WITH_LOW_ACE,
            STRAIGHT_FLUSH,
            ROYAL_FLASH
        };

        struct Occurence
        {
            Occurence(int val, int num);
            int cardValue, numberOfAKind;
        };

        struct Value
        {
            CombinationType combination;
            std::vector<Occurence> pattern;
        };

        enum { NUM_CARDS_MAX = 5 };
        explicit Hand();
        inline auto getCardCount() const noexcept { return cards_.size(); }
        void changeCard(Card card, int pos);
        inline const Card& getGard(int pos) const { return cards_.at(pos); }
        inline const Card& operator[](int pos) const { return cards_.at(pos); }
        Value getValue() const;
        std::string toString() const;
        int findIndexByValue(int val) const;
        void addCard(const Card& card);
        void clear();
        void setLowAceAtFirstPlace();

    private:
        std::vector<Card> cards_;
        mutable Value value_;
        mutable bool isValueChanged_{true};
        void evaluate() const;
    };
    int compareHandValues(const Hand::Value &val1, const Hand::Value &val2);
    std::ostream &operator<<(std::ostream &os, const Hand &hand);
}

