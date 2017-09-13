#pragma once
#include <memory>
#include <vector>

namespace poker
{
    class Model;
    class Controller
    {
    public:
        explicit Controller(std::shared_ptr<Model> spModel);
        ~Controller() {}
        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;
        Controller(Controller&&) = delete;
        Controller& operator=(Controller&&) = delete;
        void newRoundBtnClicked();
        void mouseWheelScrolledPlus();
        void mouseWheelScrolledMinus();
        void betBtnClicked();
        void changeCardsBtnClicked(const std::vector<bool> &pattern);
        void foldBtnCliked();
        void showDownBtnClicked();
    private:
        std::shared_ptr<Model> spModel_;
    };
}


