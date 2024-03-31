#pragma once

#include "Global.hpp"
#include "Window/Event/IEvent.hpp"

namespace wnd {
    /**
     * @brief Gets emitted when the user scrolls.
     */
    class ScrollEvent : public IEvent {
    public:
        [[nodiscard]] virtual constexpr const char* getName() const noexcept override { return "ScrollEvent"; }

        [[nodiscard]] explicit ScrollEvent(int8 delta) noexcept : m_delta(delta) {}

        /// @brief Returns the scroll delta, a positive delta means that the user scrolled up.
        [[nodiscard]] inline int8 getDelta() const noexcept { return m_delta; }
        /// @brief Returns true if the user scrolled up.
        [[nodiscard]] inline bool isUp() const noexcept { return m_delta > 0; }
        /// @brief Returns true if the user scrolled down.
        [[nodiscard]] inline bool isDown() const noexcept { return m_delta < 0; }

    private:
        int8 m_delta; //< The scroll delta.
    };
}