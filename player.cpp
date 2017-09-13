#include "player.h"
#include "utils.h"
#include <sstream>

poker::Player::Player(const std::string &name, double money):
    name_{name}, money_{money}, upHand_{std::make_unique<Hand>()}
{}

const std::string poker::Player::getHandTextDescription(bool isDetailed) const
{
    std::ostringstream oss;
    oss << name_ << "(" << convertToStringWithPrecision(money_, 1) <<  "$)";
    if(isDetailed)
        oss << ": " << *upHand_;
    return oss.str();
}

void poker::Player::addCard(poker::Card card)
{
    upHand_->addCard(card);
}

void poker::Player::clearCards()
{
    upHand_->clear();
}

void poker::Player::changeCard(poker::Card card, int pos)
{
    upHand_->changeCard(card, pos);
}
