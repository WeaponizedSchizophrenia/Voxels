#pragma once

#include "Global.hpp"
#include "Window/Event/IEvent.hpp"
#include <utility> // For std::pair

namespace wnd {
    /**
     * @brief Gets emitted when the cursor moves.
     */
    class MouseMotionEvent : public IEvent {
    public:
        [[nodiscard]] virtual constexpr const char* getName() const noexcept override { return "MouseMotionEvent"; }

        [[nodiscard]] explicit MouseMotionEvent(int32 x, int32 y) noexcept : m_x(x), m_y(y) {}

        /// @brief Returns the new x coordinate of the cursor.
        [[nodiscard]] inline int32 getX() const noexcept { return m_x; }
        /// @brief Returns the new y coordinate of the cursor.
        [[nodiscard]] inline int32 getY() const noexcept { return m_y; }
        /// @brief Returns the new position of the cursor.
        [[nodiscard]] inline std::pair<int32, int32> getPos() const noexcept { return { m_x, m_y }; }
    
    private:
        const int32 m_x, m_y; //< The new position of the cursor.
    };
}