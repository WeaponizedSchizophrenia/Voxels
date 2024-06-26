#pragma once

#include "Global.hpp"
#include "Window/Event/IEvent.hpp" // For the interface.
#include <utility> // For std::pair

namespace wnd {
    /**
     * @brief Gets emitted when the window is resized.
     */
    class ResizeEvent : public IEvent {
    public:
        [[nodiscard]] virtual constexpr const char* getName() const noexcept override { return "ResizeEvent"; }

        [[nodiscard]] explicit ResizeEvent(uint32 width, uint32 height) noexcept : m_width(width), m_height(height) {}

        /// @brief Returns the new width of the window.
        [[nodiscard]] inline uint32 getWidth() const noexcept { return m_width; }
        /// @brief Returns the new height of the window.
        [[nodiscard]] inline uint32 getHeight() const noexcept { return m_height; }
        /// @brief Returns the new dimensions of the window.
        [[nodiscard]] inline std::pair<uint32, uint32> getDimensions() const noexcept { return { m_width, m_height }; }

    private:
        uint32 m_width, m_height; //< The new dimensions of the window.
    };
}