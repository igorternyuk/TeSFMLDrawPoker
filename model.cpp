#include "model.h"
#include "utils.h"
#include "modellistener.h"

#include <algorithm>
#ifdef DEBUG
#include <iostream>
#endif

poker::Model::Model():
    upDeck_{std::make_unique<Deck>()},
    upHumanPlayer_{std::make_unique<Player>(humanPlayerName_, MONEY)},
    upComputerPlayer_{std::make_unique<Player>(computerPlayerName_,
                                               COMP_MONEY )}
{
    upDeck_->populate();
    upDeck_->shuffle();
}

void poker::Model::addListener(std::shared_ptr<ModelListener> spListener)
{
    listeners_.push_back(spListener);
}

void poker::Model::removeListener(std::shared_ptr<ModelListener> spListener)
{
    listeners_.remove_if([spListener](std::weak_ptr<ModelListener> p){
        return !(p.owner_before(spListener) || spListener.owner_before(p));
    });
}

void poker::Model::startNewRound()
{
    if(upHumanPlayer_->getMoney() < BET_MIN + ANTE)
    {
        gameInfo_ = std::string("Sorry, you don't have enough money!");
    }
    else if(upComputerPlayer_->getMoney() < BET_MIN + ANTE)
    {
        gameInfo_ = std::string("Sorry, computer doesn't have enough money!");
    }
    else
    {
        ++roundNumber_;
        upHumanPlayer_->payToPot(ANTE);
        upComputerPlayer_->payToPot(ANTE);
        pot_ = 2 * ANTE;
        bet_ = BET_MIN;
        upDeck_->populate();
        upDeck_->shuffle();
        upHumanPlayer_->clearCards();
        upComputerPlayer_->clearCards();
        numHumanChangedCards_ = 0;
        numComputerChangedCards_ = 0;
        for(auto i = 0u; i < Hand::NUM_CARDS_MAX; ++i)
        {
            upDeck_->deal(*upHumanPlayer_);
            upDeck_->deal(*upComputerPlayer_);
        }
        gameState_ = GameState::FIRST_STREET;
        gameInfo_ = "Make you bets!";
#ifdef DEBUG
    std::cout << upHumanPlayer_->getHandTextDescription() << std::endl;
    std::cout << upComputerPlayer_->getHandTextDescription() << std::endl;
#endif
    }
    notifyAllListeners();
}

void poker::Model::increaseBet()
{
    if(gameState_ == GameState::FIRST_STREET ||
       gameState_ == GameState::SECOND_STREET)
    {
        if(bet_ < upHumanPlayer_->getMoney() / 2 - BET_STEP)
        {
            bet_ += BET_STEP;
            notifyAllListeners();
#ifdef DEBUG
            std::cout << "Увеличиваем ставку - " << bet_ << std::endl;
#endif
        }
    }
}

void poker::Model::decreaseBet()
{
    if(gameState_ == GameState::FIRST_STREET ||
       gameState_ == GameState::SECOND_STREET)
    {
        if(bet_ > BET_MIN + BET_STEP)
        {
            bet_ -= BET_STEP;
            notifyAllListeners();
#ifdef DEBUG
            std::cout << "Уменьшаем ставку - " << bet_ << std::endl;
#endif
        }
    }
}

void poker::Model::makeBet()
{
    if(gameState_ == GameState::FIRST_STREET ||
       gameState_ == GameState::SECOND_STREET)
    {
        pot_ += 2 * bet_;
        upHumanPlayer_->payToPot(bet_);
        upComputerPlayer_->payToPot(bet_);
        if(gameState_ == GameState::FIRST_STREET)
        {
            gameState_ = GameState::HAND_IMPROVING;
            gameInfo_ = std::string("First bets were made. Players can change their cards.");
        }
        else
        {
            gameState_ = GameState::SHOW_DOWN;
            gameInfo_ = std::string("Second bets were made. Players show down threir cards.");
        }
        bet_ = BET_MIN;
        notifyAllListeners();
    }
}

const poker::Card poker::Model::getHumanPlayerCard(int pos) const
{
    return upHumanPlayer_->getCard(pos);
}

const std::string poker::Model::getHumanPlayerTextDescription() const
{
    return upHumanPlayer_->getHandTextDescription();
}

const std::string poker::Model::getComputerPlayerTextDescription() const
{
    return isComputerCardsFaceUp() ?
                upComputerPlayer_->getHandTextDescription() :
           upComputerPlayer_->getHandTextDescription(false);
}

const poker::Card poker::Model::getComputerPlayerCard(int pos) const
{
    return upComputerPlayer_->getCard(pos);
}

bool poker::Model::isComputerCardsFaceUp() const
{
    return gameState_ == GameState::HUMAN_WON ||
            gameState_ == GameState::COMPUTER_WON ||
            gameState_ == GameState::PUSH;
}

const std::string poker::Model::getPot() const
{
    return std::string("Pot:") + convertToStringWithPrecision(pot_, 1) +
            std::string("$");
}

const std::string poker::Model::getBet() const
{
    return std::string("Bet:") + convertToStringWithPrecision(bet_, 1) +
            std::string("$");
}

const std::string poker::Model::getRoundNumber() const
{
    return std::string("Round: ") + std::to_string(roundNumber_);
}

void poker::Model::changeHumanPlayerCards(const std::vector<bool> &pattern)
{
    if(gameState_ == GameState::HAND_IMPROVING)
    {
        for(auto i = 0u; i < pattern.size(); ++i)
        {
            if(pattern[i])
            {
                upHumanPlayer_->changeCard(upDeck_->top(), i);
                ++numHumanChangedCards_;
            }
        }
        changeComputerPlayerCards();
        gameInfo_ = upHumanPlayer_->getName() + " has changed " +
                    std::to_string(numHumanChangedCards_) +
                    std::string(" cards.\n") +
                    upComputerPlayer_->getName() + " has changed " +
                    std::to_string(numComputerChangedCards_) +
                    std::string(" cards.");
        gameState_ = GameState::SECOND_STREET;
        notifyAllListeners();
    }
}

void poker::Model::fold()
{
    gameState_ = GameState::COMPUTER_WON;
    upComputerPlayer_->takeGain(pot_);
    gameInfo_ = upHumanPlayer_->getName() + std::string(" folded.\n") +
    upComputerPlayer_->getName() + std::string(" won.");
    notifyAllListeners();
}

void poker::Model::showDown()
{
    decideWinner();
}

void poker::Model::decideWinner()
{
    if(gameState_ == GameState::SHOW_DOWN)
    {
        int res{compareHandValues(upHumanPlayer_->getHandValue(),
                                  upComputerPlayer_->getHandValue())};
        std::cout << "Compare hands: " << res << std::endl;
        switch(res)
        {
            case 1:
                upHumanPlayer_->takeGain(pot_);
                gameState_ = GameState::HUMAN_WON;
                gameInfo_ = upHumanPlayer_->getName() + std::string(" won.");
                break;
            case -1:
                upComputerPlayer_->takeGain(pot_);
                gameState_ = GameState::COMPUTER_WON;
                gameInfo_ = upComputerPlayer_->getName() + std::string(" won.");
                break;
            case 0:
                upHumanPlayer_->takeGain(pot_ / 2);
                upComputerPlayer_->takeGain(pot_ / 2);
                gameInfo_ = std::string("Push!The pot will be divided.");
                gameState_ = GameState::PUSH;
                break;
            default:
                break;
        }
        pot_ = 0; // Обнуляем банк
        notifyAllListeners();
    }
}

void poker::Model::changeComputerPlayerCards()
{
    auto c = upComputerPlayer_->getHandValue().combination;
    auto pattern = upComputerPlayer_->getHandValue().pattern;
    if(c == Hand::CombinationType::HIGH_CARD)
    {
        for(auto i = 0; i < NUM_LOWEST_CARDS_TO_DISCARD; ++i)
        {
            upComputerPlayer_->changeCard(upDeck_->top(), i);
            ++numComputerChangedCards_;
        }
    }
    else if(c == Hand::CombinationType::PAIR ||
            c == Hand::CombinationType::TWO_PAIR ||
            c == Hand::CombinationType::THREE_OF_A_KIND)
    {
        for(auto i = 1u; i < pattern.size(); ++i)
        {
            if(pattern[i].numberOfAKind == 1)
            {
                int val = upComputerPlayer_->getHand()->
                          findIndexByValue(pattern[i].cardValue);
                upComputerPlayer_->changeCard(upDeck_->top(), val);
                ++numComputerChangedCards_;
            }
        }
    }
    notifyAllListeners();
}

void poker::Model::notifyAllListeners()
{
    for(const auto &l: listeners_)
    {
        if(!l.expired())
        {
            auto spListener = l.lock();
            spListener->updateView();
        }
    }
}


