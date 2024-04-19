#pragma once

#include "Global.hpp"
namespace renderer {
    enum class TextureWrap : uint8 {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };
}