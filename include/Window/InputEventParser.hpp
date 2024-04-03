#pragma once

#include "Window/Event/IEvent.hpp"
#include "Window/Event/Key.hpp"
#include "Window/Event/KeyboardEvent.hpp"
#include "Window/Event/MouseButtonEvent.hpp"
#include <bitset>

namespace wnd {
    /**
     * @brief Parses input events and stores the state in the parser. Which can be easily accessed through methods.
     */
    class InputEventParser {
    public:
        /**
         * @brief Holds the modifier state.
         */
        struct ModifierState {
            bool m_leftCtrl = false; //< Indicates whether or not the left control key is currently pressed.
            bool m_rightCtrl = false; //< Indicates whether or not the right control key is currently pressed.
            bool m_leftShift = false; //< Indicates whether or not the left shift key is currently pressed.
            bool m_rightShift = false; //< Indicates whether or not the right shift key is currently pressed.
        };


        /**
         * @brief Parses the event and stores the state in the parser.
         * 
         * @param event The event to parse.
         */
        void parseEvent(const wnd::IEvent& event) noexcept {
            if(auto* keyEvent = event.downCast<wnd::KeyboardEvent>(); keyEvent) {

                if(keyEvent->isPressed()) {
                    m_pressedKeys.set(static_cast<uint8>(keyEvent->getKey()));

                    switch(keyEvent->getKey()) {
                        case wnd::Key::LeftControl:     { m_modState.m_leftCtrl = true; }
                        case wnd::Key::RightControl:    { m_modState.m_rightCtrl = true; }
                        case wnd::Key::LeftShift:       { m_modState.m_leftShift = true; }
                        case wnd::Key::RightShift:      { m_modState.m_rightShift = true; }
                        default: {  }
                    }

                } else {
                    m_releasedKeys.set(static_cast<uint8>(keyEvent->getKey()));

                    switch(keyEvent->getKey()) {
                        case wnd::Key::LeftControl:     { m_modState.m_leftCtrl = false; }
                        case wnd::Key::RightControl:    { m_modState.m_rightCtrl = false; }
                        case wnd::Key::LeftShift:       { m_modState.m_leftShift = false; }
                        case wnd::Key::RightShift:      { m_modState.m_rightShift = false; }
                        default: {  }
                    }

                }

            } else if(auto* mbEvent = event.downCast<wnd::MouseButtonEvent>(); mbEvent) {
                
                if(mbEvent->isPressed()) {
                    m_pressedMouseButtons.set(static_cast<uint8>(mbEvent->getButton()));
                } else {
                    m_releasedMouseButtons.set(static_cast<uint8>(mbEvent->getButton()));
                }

            }
        }

        /**
         * @brief Resets the state.
         */
        void onLoopEnded() noexcept {
            m_pressedKeys.reset();
            m_releasedKeys.reset();
            m_pressedMouseButtons.reset();
            m_releasedMouseButtons.reset();
        }

        /**
         * @brief Returs whether or not the key was pressed.
         * 
         * @param key The key to check.
         * @return true If the key was pressed.
         * @return false If the key was not pressed.
         */
        [[nodiscard]] inline bool isPressed(wnd::Key key) const noexcept { return m_pressedKeys.test(static_cast<uint8>(key)); }
        /**
         * @brief Returns whether or not the key was released.
         * 
         * @param key The key to check.
         * @return true If the key was released.
         * @return false If the key was not released.
         */
        [[nodiscard]] inline bool isReleased(wnd::Key key) const noexcept { return m_releasedKeys.test(static_cast<uint8>(key)); }

        /**
         * @brief Returns whether or not the mouse button was pressed.
         * 
         * @param button The mouse button to check.
         * @return true If the mouse button was pressed.
         * @return false If the mouse button was not pressed.
         */
        [[nodiscard]] inline bool isPressed(wnd::MouseButtonEvent::Button button) const noexcept { 
            return m_pressedMouseButtons.test(static_cast<uint8>(button)); }
        /**
         * @brief Returns whether or not the mouse button was released.
         * 
         * @param button The mouse button to check.
         * @return true If the mouse button was released.
         * @return false If the mouse button was not released.
         */
        [[nodiscard]] inline bool isReleased(wnd::MouseButtonEvent::Button button) const noexcept { 
            return m_releasedMouseButtons.test(static_cast<uint8>(button)); }

        /**
         * @brief Returns a reference to the modifier state.
         * 
         * @return const ModifierState& The modifier state.
         */
        [[nodiscard]] inline const ModifierState& getModifierState() const noexcept { return m_modState; }

    private:
        std::bitset<0xFF> m_pressedKeys; //< Bitset for all the pressed keys.
        std::bitset<0xFF> m_releasedKeys; //< Bitset for all the released keys.
        std::bitset<5> m_pressedMouseButtons; //< Bitset for all the pressed mouse buttons.
        std::bitset<5> m_releasedMouseButtons; //< Bitset for all the released mouse buttons.
        ModifierState m_modState; //< The modifier state.
    };
}