#include "controller.h"
#include "model.h"

poker::Controller::Controller(std::shared_ptr<poker::Model> spModel):
    spModel_(spModel)
{}

void poker::Controller::onNewRoundBtnClicked()
{
    spModel_->startNewRound();
}

void poker::Controller::onMouseWheelScrolledPlus()
{
    spModel_->increaseBet();
}

void poker::Controller::onMouseWheelScrolledMinus()
{
    spModel_->decreaseBet();
}

void poker::Controller::onMakeBetBtnClicked()
{
    spModel_->makeBet();
}

void poker::Controller::onChangeCardsBtnClicked(const std::vector<bool> &pattern)
{
    spModel_->changeHumanPlayerCards(pattern);
}

void poker::Controller::onFoldBtnCliked()
{
    spModel_->fold();
}

void poker::Controller::onShowDownBtnClicked()
{
    spModel_->showDown();
}
