#pragma once

#include "Global.hpp"
#include <random>

namespace world {
    using Id = uint64;

    /**
     * @brief Generates a radnom 64 bit id.
     * 
     * @return uint64 The generated id.
     */
    [[nodiscard]] static Id generateId() noexcept {
        static std::uniform_int_distribution<Id> distribution;
        static std::random_device device;
        static std::mt19937 engine(device());

        return distribution(engine);
    }
}