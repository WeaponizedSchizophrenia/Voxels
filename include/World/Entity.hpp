#pragma once

#include "World/World.hpp"

namespace world {
    /**
     * @brief The base class for all scene objects.
     */
    class Entity {
    public:
        Entity() noexcept : m_id(generateId()) {  }

        /**
         * @brief Gets the id of the entity.
         * 
         * @return Id The id.
         */
        [[nodiscard]] inline Id getId() const noexcept { return m_id; }

    private:
        const Id m_id; //< The entity id.
    };
}