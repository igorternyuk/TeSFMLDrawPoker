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
        void onNewRoundBtnClicked();
        void onMouseWheelScrolledPlus();
        void onMouseWheelScrolledMinus();
        void onMakeBetBtnClicked();
        void onChangeCardsBtnClicked(const std::vector<bool> &pattern);
        void onFoldBtnCliked();
        void onShowDownBtnClicked();
    private:
        std::shared_ptr<Model> spModel_;
    };
}


