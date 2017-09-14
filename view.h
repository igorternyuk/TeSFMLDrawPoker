#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "card.h"
#include "modellistener.h"
#include <string>
#include <memory>

namespace poker
{
    class ConstantModelInterface;
    class Controller;

    class View: public ModelListener
    {
    public:
        explicit View(std::shared_ptr<ConstantModelInterface> spConstantModel,
                      std::shared_ptr<Controller> spController);
        ~View() {}
        View(const View&) = delete;
        View& operator=(const View&) = delete;
        View(View&&) = delete;
        View& operator=(View&&) = delete;
        void run();
    private:
        const std::string WINDOW_TITLE{"TeFiveCardDrawPoker"};        
        const sf::Color m_bgColor{sf::Color(30, 174, 124)};
        enum
        {
            WINDOW_WIDTH = 800,
            WINDOW_HEIGHT = 600,
            CARD_WIDTH = 73,
            CARD_HEIGHT = 98,
            CARD_STEP = 5,
            BUTTON_ROW_LEFT = 5,
            BUTTON_ROW_TOP = 540,
            BUTTON_STEP = 130
        };
        const std::vector<sf::IntRect> cardPositions_
        {
          {205, 355, CARD_WIDTH + 1, CARD_HEIGHT + 1},
          {283, 355, CARD_WIDTH + 1, CARD_HEIGHT + 1},
          {361, 355, CARD_WIDTH + 1, CARD_HEIGHT + 1},
          {439, 355, CARD_WIDTH + 1, CARD_HEIGHT + 1},
          {517, 355, CARD_WIDTH + 1, CARD_HEIGHT + 1}
        };
        std::shared_ptr<ConstantModelInterface> spConstantModel_;
        std::shared_ptr<Controller> spController_;
        sf::RenderWindow window_;
        sf::Font font_;
        sf::Sprite sCards_, sCardBack_;
        sf::Texture tCards_, tCardBack_;
        sf::Text title_;
        sf::Text txtHumanPlayer_, txtComputerPlayer_;
        sf::Text txtGameInfo_, txtPot_, txtBet_, txtRound_;
        std::vector<sf::Sprite> btnSprites_;
        std::vector<sf::Texture> btnTextures_;
        std::vector<bool> cardsToDiscardPattern_
        {false, false, false, false, false};
        bool draw_{true};
        sf::Music bgm_;
        virtual inline void updateView() { draw_ = true; }

        void loadImages();
        void loadFonts();
        void loadSounds();
        void userInputPhase();
        void drawingPhase();
        void drawCard(Card card, int posX, int posY);
        void drawClosedCard(int posX, int posY);
        void drawCards();
        void drawButtons();
        void drawGameTextInfo();
        void drawContourAroundSelectedCards();
        void configureTextLables();
        void cancelSelection();
    };
}



