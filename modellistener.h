#pragma once

namespace poker {

    class ModelListener
    {
    public:
        virtual ~ModelListener() {}
        virtual void updateView() = 0;
    };

}
