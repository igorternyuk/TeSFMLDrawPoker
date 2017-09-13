#include "view.h"
#include "constantmodelinterface.h"
#include "controller.h"
#include <memory>
#ifdef DEBUG
#include <iostream>
#endif

poker::View::View(std::shared_ptr<poker::ConstantModelInterface> spConstantModel,
                  std::shared_ptr<poker::Controller> spController):
    spConstantModel_{spConstantModel},
    spController_{spController},
    window_{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE}
{
    const unsigned int screenWidth{sf::VideoMode::getDesktopMode().width};
    const unsigned int screenHeight{sf::VideoMode::getDesktopMode().height};
    window_.setPosition(sf::Vector2i{(screenWidth - WINDOW_WIDTH) / 2,
                                     (screenHeight - WINDOW_HEIGHT) / 2}
                        );
    spController_->newRoundBtnClicked();
    loadFonts();
    loadImages();
    configureTextLables();
    loadSounds();
}

void poker::View::run()
{
    bgm_.play();
    while (window_.isOpen())
    {
        userInputPhase();
        drawingPhase();
    }
}

void poker::View::userInputPhase()
{
    sf::Event event;
    while (window_.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
        {
            window_.close();
        }
        else if(event.type == sf::Event::Resized)
        {
            updateView();
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Escape:
                    window_.close();
                    break;
                case sf::Keyboard::N:
                    cancelSelection();
                    spController_->newRoundBtnClicked();
                    break;
                case sf::Keyboard::C:
                    spController_->changeCardsBtnClicked(cardsToDiscardPattern_);
                    break;
                case sf::Keyboard::Up:
                    spController_->mouseWheelScrolledPlus();
                    break;
                case sf::Keyboard::Down:
                    spController_->mouseWheelScrolledMinus();
                    break;
                case sf::Keyboard::B:
                    spController_->betBtnClicked();
                    break;
                case sf::Keyboard::F:
                    spController_->foldBtnCliked();
                    break;
                case sf::Keyboard::S:
                    spController_->showDownBtnClicked();
                    break;
                default:
                        break;
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            int cursorX{event.mouseButton.x};
            int cursorY{event.mouseButton.y};
#ifdef DEBUG
            std::cout << "cursor -> x: " << cursorX << " y: " << cursorY <<
                         std::endl;
#endif
            if(spConstantModel_->getGameState() == GameState::HAND_IMPROVING)
            {
                for(int i = 0; i < spConstantModel_->getCardsNumberMax(); ++i)
                {
                    if(cardPositions_[i].contains(cursorX, cursorY))
                    {
                        cardsToDiscardPattern_[i].flip();
                        updateView();
                        //cardsToDiscardPattern_[i] = !cardsToDiscardPattern_[i];
                        break;
                    }
                }
#ifdef DEBUG
                for(auto p: cardsToDiscardPattern_)
                    std::cout << p << "-";
                std::cout << std::endl;
#endif
            }

            for(auto i = 0u; i < btnSprites_.size(); ++i)
            {
                if(btnSprites_[i].getGlobalBounds().contains(cursorX,
                                                             cursorY))
                {
                    switch(i)
                    {
                        case 0:
                            cancelSelection();
                            spController_->newRoundBtnClicked();
                            break;
                        case 1:
                            spController_->betBtnClicked();
                            break;
                        case 2:
                            spController_->changeCardsBtnClicked(
                                    cardsToDiscardPattern_);
                            break;
                        case 3:
                            spController_->foldBtnCliked();
                            break;
                        case 4:
                            spController_->showDownBtnClicked();
                            break;
                        case 5:
                            window_.close();
                            break;
                    }
                }
            }
        }
        else if(event.type == sf::Event::MouseWheelScrolled)
        {
            float delta{event.mouseWheelScroll.delta};
            if(delta > 0)
                spController_->mouseWheelScrolledPlus();
            else if(delta < 0)
                spController_->mouseWheelScrolledMinus();
        }
    }
}

void poker::View::drawingPhase()
{
    //Drawing stuff

    if(draw_)
    {
        window_.clear(m_bgColor);
        window_.draw(title_);
        //Draw cards
        drawCards();
        //Draw buttons
        drawButtons();
        //Draw game info
        drawGameTextInfo();
        //Draw selection
        drawContourAroundSelectedCards();
        window_.display();
        draw_ = false;
    }
}

void poker::View::drawCard(Card card, int posX, int posY)
{
    auto val = card.getValue();
    int distRectX = val == Card::HIGHEST_CARD_VALUE ? 0 : card.getValue() - 1;
    int distRectY = static_cast<int>(card.getSuit());
    sCards_.setTextureRect(sf::IntRect(distRectX * CARD_WIDTH,
                                       distRectY * CARD_HEIGHT,
                                       CARD_WIDTH + 1, CARD_HEIGHT + 1));
    sCards_.setPosition(posX, posY);
    window_.draw(sCards_);
}

void poker::View::drawClosedCard(int posX, int posY)
{
    sCardBack_.setPosition(posX, posY);
    window_.draw(sCardBack_);
}

void poker::View::drawCards()
{
    static const int cardRowsLeft = 205, computerPlayerCardRowTop = 140;

    for(int i = 0; i < spConstantModel_->getCardsNumberMax(); ++i)
    {
        drawCard(spConstantModel_->getHumanPlayerCard(i),
                 cardPositions_[i].left, cardPositions_[i].top);
        if(spConstantModel_->isComputerCardsFaceUp())
        {
            drawCard(spConstantModel_->getComputerPlayerCard(i),
                     cardRowsLeft + (CARD_WIDTH + CARD_STEP) * i,
                     computerPlayerCardRowTop);
        }
        else
        {
            drawClosedCard(cardRowsLeft + (CARD_WIDTH + CARD_STEP) * i,
                           computerPlayerCardRowTop);
        }
    }

}

void poker::View::drawButtons()
{
    for(const auto &s: btnSprites_)
        window_.draw(s);
}

void poker::View::drawGameTextInfo()
{
    //Text labels positioning constants
    static const int txtBetTop = 495, txtPotTop = 250, txtGameInfoTop = 285,
                     txtRoundTop = 55, txtHumanPlayerTop = 460,
                     txtComputerPlayerTop = 85;

    txtGameInfo_.setString(spConstantModel_->getGameInfo());
    txtGameInfo_.setPosition(
    (WINDOW_WIDTH - txtGameInfo_.getGlobalBounds().width) / 2, txtGameInfoTop);

    txtBet_.setString(spConstantModel_->getBet());
    txtBet_.setPosition((WINDOW_WIDTH - txtBet_.getGlobalBounds().width) / 2,
                        txtBetTop);
    txtPot_.setString(spConstantModel_->getPot());
    txtPot_.setPosition((WINDOW_WIDTH - txtPot_.getGlobalBounds().width) / 2,
                        txtPotTop);
    txtRound_.setString(spConstantModel_->getRoundNumber());
    txtRound_.setPosition((WINDOW_WIDTH - txtRound_.getGlobalBounds().width) / 2,
                        txtRoundTop);

    txtHumanPlayer_.setString(spConstantModel_->getHumanPlayerTextDescription());
    float w1{txtHumanPlayer_.getGlobalBounds().width};
    txtHumanPlayer_.setPosition((WINDOW_WIDTH - w1) / 2, txtHumanPlayerTop);

    txtComputerPlayer_.setString(spConstantModel_->
                                 getComputerPlayerTextDescription());
    float w2{txtComputerPlayer_.getGlobalBounds().width};
    txtComputerPlayer_.setPosition((WINDOW_WIDTH - w2) / 2,
                                 txtComputerPlayerTop);

    window_.draw(txtGameInfo_);
    window_.draw(txtPot_);
    window_.draw(txtBet_);
    window_.draw(txtRound_);
    window_.draw(txtHumanPlayer_);
    window_.draw(txtComputerPlayer_);
}

void poker::View::drawContourAroundSelectedCards()
{
    if(spConstantModel_->getGameState() == GameState::HAND_IMPROVING)
    {
        for(int i = 0; i < spConstantModel_->getCardsNumberMax(); ++i)
        {
            if(cardsToDiscardPattern_[i])
            {
                sf::RectangleShape outline({CARD_WIDTH + 1,
                                            CARD_HEIGHT + 1});
                outline.setFillColor(sf::Color(0, 0, 0, 0));
                outline.setOutlineThickness(2);
                outline.setOutlineColor(sf::Color::Red);
                outline.setPosition(cardPositions_[i].left,
                                    cardPositions_[i].top);
                window_.draw(outline);
            }
        }
    }
}

void poker::View::configureTextLables()
{
    //Some constants
    static const int titleFontSize{55}, textLabelsFontSize{30};

    title_.setFont(font_);
    title_.setString("Draw poker");
    title_.setCharacterSize(titleFontSize);
    title_.setPosition((WINDOW_WIDTH - title_.getGlobalBounds().width) / 2, 1);
    title_.setColor(sf::Color(127, 0, 0));

    txtHumanPlayer_.setFont(font_);
    txtHumanPlayer_.setColor(sf::Color::Yellow);
    txtHumanPlayer_.setCharacterSize(textLabelsFontSize);

    txtComputerPlayer_.setFont(font_);
    txtComputerPlayer_.setColor(sf::Color::Yellow);
    txtComputerPlayer_.setCharacterSize(textLabelsFontSize);

    //Game info

    txtGameInfo_.setFont(font_);
    txtGameInfo_.setColor(sf::Color::Red);
    txtGameInfo_.setCharacterSize(textLabelsFontSize);
    txtGameInfo_.setString(spConstantModel_->getGameInfo());

    //Pot

    txtPot_.setFont(font_);
    txtPot_.setColor(sf::Color::Yellow);
    txtPot_.setCharacterSize(textLabelsFontSize);
    txtPot_.setString(spConstantModel_->getPot());

    //Bet

    txtBet_.setFont(font_);
    txtBet_.setColor(sf::Color::Red);
    txtBet_.setCharacterSize(textLabelsFontSize);
    txtBet_.setString(spConstantModel_->getBet());

    //Round number

    txtRound_.setFont(font_);
    txtRound_.setColor(sf::Color::Blue);
    txtRound_.setCharacterSize(textLabelsFontSize);
    txtRound_.setString(spConstantModel_->getRoundNumber());
}

void poker::View::loadImages()
{
    static const std::string PATH_TO_CARD_SET{"resources/gfx/CardsSprite.png"};
    static const std::string PATH_TO_CARD_BACK_IMAGE{"resources/gfx/"
                                                     "cardBackPokerBlue.png"};
    static const std::vector<std::string> PATHES_TO_BTN_IMAGES
    {
      {"resources/gfx/NewRound.png"},
      {"resources/gfx/Bet.png"},
      {"resources/gfx/ChangeCards.png"},
      {"resources/gfx/Flop.png"},
      {"resources/gfx/ShowDown.png"},
      {"resources/gfx/Quit.png"}
    };
    tCards_.loadFromFile(PATH_TO_CARD_SET);
    tCards_.setSmooth(true);
    sCards_.setTexture(tCards_);
    tCardBack_.loadFromFile(PATH_TO_CARD_BACK_IMAGE);
    tCardBack_.setSmooth(true);
    sCardBack_.setTexture(tCardBack_);

    for(auto i = 0u; i < PATHES_TO_BTN_IMAGES.size(); ++i)
    {
        sf::Texture texture;
        texture.loadFromFile(PATHES_TO_BTN_IMAGES[i]);
        texture.setSmooth(true);
        btnTextures_.push_back(texture);
    }

    for(auto i = 0u; i < btnTextures_.size(); ++i)
    {
        sf::Sprite sprite;
        sprite.setTexture(btnTextures_[i]);
        sprite.setPosition(BUTTON_ROW_LEFT + BUTTON_STEP * i, BUTTON_ROW_TOP);
        btnSprites_.push_back(sprite);
    }
}

void poker::View::loadFonts()
{
    static const std::string pathToFont{"resources/fonts/BRLNSR.TTF"};
    font_.loadFromFile(pathToFont);
}

void poker::View::loadSounds()
{
    static const std::string pathToBGM{"resources/sounds/bgm_casino.ogg"};
    bgm_.openFromFile(pathToBGM);
    bgm_.setLoop(true);
}

void poker::View::cancelSelection()
{
    for(auto i = 0u; i < cardsToDiscardPattern_.size(); ++i)
        cardsToDiscardPattern_[i] = false;
}
