#pragma once

#include "hand.h"
#include <vector>
#include <string>
#include <memory>

namespace poker
{
    class Player
    {
    public:
        explicit Player(const std::string &name, double money);
        inline const std::string& getName() const noexcept { return name_; }
        inline auto getMoney() const noexcept { return money_; }
        inline auto getHandValue() const { return upHand_->getValue(); }
        inline auto getCard(int pos) const { return upHand_->getGard(pos); }
        inline auto getCardCount() const { return upHand_->getCardCount(); }
        const std::string getHandTextDescription(bool isDetailed = true) const;
        inline const std::unique_ptr<Hand>& getHand() const { return upHand_; }
        inline void payToPot(double bet) { money_ -= bet; }
        inline void takeGain(double gain) { money_ += gain; }
        void addCard(Card card);
        void changeCard(poker::Card card, int pos);
        void clearCards();
    private:
        std::string name_;
        double money_;
        std::unique_ptr<Hand> upHand_;
    };
}

