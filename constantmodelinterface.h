#pragma once

#include "card.h"
#include <vector>
#include <string>

namespace poker
{

enum class GameState
{
    FIRST_STREET,
    HAND_IMPROVING,
    SECOND_STREET,
    SHOW_DOWN,
    HUMAN_WON,
    COMPUTER_WON,
    PUSH
};

class ConstantModelInterface
{
public:
   virtual ~ConstantModelInterface() {}
   virtual GameState getGameState() const = 0;
   virtual int getCardsNumberMax() const = 0;
   virtual const Card getHumanPlayerCard(int pos) const = 0;
   virtual const Card getComputerPlayerCard(int pos) const = 0;
   virtual const std::string getHumanPlayerTextDescription() const = 0;
   virtual const std::string getComputerPlayerTextDescription() const = 0;
   virtual bool isComputerCardsFaceUp() const = 0;
   virtual const std::string getPot() const = 0;
   virtual const std::string getBet() const = 0;
   virtual const std::string getGameInfo() const = 0;
   virtual const std::string getRoundNumber() const = 0;
};

}
