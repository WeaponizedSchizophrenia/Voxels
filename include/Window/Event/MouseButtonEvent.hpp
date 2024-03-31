#pragma once

#include "Global.hpp"
#include "Window/Event/IEvent.hpp"

namespace wnd {
    /**
     * @brief Gets emitted when a mouse button is either pressed or released.
     * 
     */
    class MouseButtonEvent : public IEvent {
    public:
        [[nodiscard]] virtual constexpr const char* getName() const noexcept override { return "MouseButtonEvent"; }

        /**
         * @brief Mouse button enum.
         */
        enum class Button : uint8 {
            Left = 1, Middle = 2, Right = 3, Side1 = 4, Side2 = 5
        };

        explicit MouseButtonEvent(Button button, ButtonState state) noexcept 
            : m_button(button), m_state(state) {}

        /// @brief Returns the button that caused the event.
        [[nodiscard]] inline Button getButton() const noexcept { return m_button; }
        /// @brief Returns the state of the button.
        [[nodiscard]] inline ButtonState getState() const noexcept { return m_state; }
        /// @brief Returns true if the button is pressed.
        [[nodiscard]] inline bool isPressed() const noexcept { return m_state == ButtonState::Pressed; }
        /// @brief Returns true if the button is released.
        [[nodiscard]] inline bool isReleased() const noexcept { return m_state == ButtonState::Released; }

    private:
        const Button m_button; //< The button that caused the event.
        const ButtonState m_state; //< The state of the button.
    };
}