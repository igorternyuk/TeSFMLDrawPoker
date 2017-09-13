#pragma once

#include "deck.h"
#include "player.h"
#include "constantmodelinterface.h"
#include <memory>
#include <string>
#include <vector>
#include <list>

namespace poker
{
    class Deck;
    class Player;
    class ModelListener;
    class Model: public ConstantModelInterface
    {
    public:
        explicit Model();
        void addListener(std::shared_ptr<ModelListener> spListener);
        void removeListener(std::shared_ptr<ModelListener> spListener);
        //Constant model interface
        virtual inline GameState getGameState() const { return gameState_; }
        virtual inline int getCardsNumberMax() const
        { return Hand::NUM_CARDS_MAX; }
        virtual const Card getHumanPlayerCard(int pos) const;
        virtual const Card getComputerPlayerCard(int pos) const;
        virtual const std::string getHumanPlayerTextDescription() const;
        virtual const std::string getComputerPlayerTextDescription() const;
        virtual bool isComputerCardsFaceUp() const;
        virtual const std::string getPot() const;
        virtual const std::string getBet() const;
        virtual const std::string getGameInfo() const { return gameInfo_; }
        virtual const std::string getRoundNumber() const;
        //Controller interface
        void startNewRound();
        void increaseBet();
        void decreaseBet();
        void makeBet();
        void changeHumanPlayerCards(const std::vector<bool> &pattern);
        void fold();
        void showDown();
    private:
        const std::string humanPlayerName_{"You"};
        const std::string computerPlayerName_{"Computer"};
        enum
        {
            ANTE = 13,
            MONEY = 1000,
            BET_MIN = 100,
            BET_STEP = 5,
            NUM_LOWEST_CARDS_TO_DISCARD = 3
        };
        std::unique_ptr<Deck> upDeck_;
        std::unique_ptr<Player> upHumanPlayer_, upComputerPlayer_;
        double pot_{0}, bet_{BET_MIN};
        int roundNumber_{0};
        GameState gameState_;
        std::string gameInfo_;
        int numHumanChangedCards_{0};
        int numComputerChangedCards_{0};
        std::list<std::weak_ptr<ModelListener>> listeners_;
        void changeComputerPlayerCards();
        void decideWinner();
        void notifyAllListeners();
    };
}
