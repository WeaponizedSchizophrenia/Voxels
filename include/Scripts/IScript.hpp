#pragma once

#include "Global.hpp"

namespace scripts {
    class IScript {
    public:
        virtual void onUpdate(float64 deltaTime) = 0;
    };
}