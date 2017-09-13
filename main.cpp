#include "model.h"
#include "controller.h"
#include "view.h"
#include <memory>

using namespace poker;
int main()
{
    auto spModel{std::make_shared<Model>()};
    auto spController{std::make_shared<Controller>(spModel)};
    auto spView{std::make_shared<View>(spModel, spController)};
    spModel->addListener(spView);
    spView->run();
    return 0;
}
