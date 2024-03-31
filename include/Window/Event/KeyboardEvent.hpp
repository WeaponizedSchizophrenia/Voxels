#pragma once

#include "Window/Event/IEvent.hpp"
#include "Window/Event/Key.hpp"

namespace wnd {
    /**
     * @brief Gets emitted when a key is either pressed or released.
     */
    class KeyboardEvent : public IEvent {
    public:
        [[nodiscard]] virtual constexpr const char* getName() const noexcept override { return "KeyboardEvent"; }

        [[nodiscard]] explicit KeyboardEvent(Key key, ButtonState state) noexcept
            : m_key(key), m_state(state) {}

        /// @brief Returns the key that caused the event.
        [[nodiscard]] inline Key getKey() const noexcept { return m_key; }
        /// @brief Returns the state of the button.
        [[nodiscard]] inline ButtonState getState() const noexcept { return m_state; }
        /// @brief Returns true if the button is pressed.
        [[nodiscard]] inline bool isPressed() const noexcept { return m_state == ButtonState::Pressed; }
        /// @brief Returns true if the button is released.
        [[nodiscard]] inline bool isReleased() const noexcept { return m_state == ButtonState::Released; }

    private:
        const Key m_key; //< The key that caused the event.
        const ButtonState m_state; //< The state of the button.
    };
}