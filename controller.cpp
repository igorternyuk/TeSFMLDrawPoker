#include "controller.h"
#include "model.h"

poker::Controller::Controller(std::shared_ptr<poker::Model> spModel):
    spModel_(spModel)
{}

void poker::Controller::newRoundBtnClicked()
{
    spModel_->startNewRound();
}

void poker::Controller::mouseWheelScrolledPlus()
{
    spModel_->increaseBet();
}

void poker::Controller::mouseWheelScrolledMinus()
{
    spModel_->decreaseBet();
}

void poker::Controller::betBtnClicked()
{
    spModel_->makeBet();
}

void poker::Controller::changeCardsBtnClicked(const std::vector<bool> &pattern)
{
    spModel_->changeHumanPlayerCards(pattern);
}

void poker::Controller::foldBtnCliked()
{
    spModel_->fold();
}

void poker::Controller::showDownBtnClicked()
{
    spModel_->showDown();
}
