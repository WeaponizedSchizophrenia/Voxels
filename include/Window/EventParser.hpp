#pragma once

#include "Observer.hpp" // For the emmitter.
#include "Window/Event/CloseRequestedEvent.hpp" // For handling the event.
#include "Window/Event/IEvent.hpp" // For handling the event.
#include "Window/Event/Key.hpp" // For storing the keys.
#include "Window/Event/KeyboardEvent.hpp" // For handling the event.
#include "Window/Event/MouseButtonEvent.hpp" // For handling the event.
#include "Window/Event/MouseMotionEvent.hpp" // For handling the event.
#include "Window/Event/ResizeEvent.hpp" // For handling the event.
#include "Window/Event/ScrollEvent.hpp" // For handling the event.
#include <bitset> // For the key data storage.
#include <memory> // For the smart pointers.

namespace wnd {
    /**
     * @brief Parses input events and stores the state in the parser. Which can be easily accessed through methods.
     */
    class EventParser {
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

            } else if(auto* scrollEvent = event.downCast<wnd::ScrollEvent>(); scrollEvent) {
                CursorScrolled->emit(*scrollEvent);
            } else if(auto* resizeEvent = event.downCast<wnd::ResizeEvent>(); resizeEvent) {
                WindowResized->emit(*resizeEvent);
            } else if(auto* motionEvent = event.downCast<wnd::MouseMotionEvent>(); motionEvent) {
                int32 deltaX = motionEvent->getX() - m_lastCursorPos.x;
                int32 deltaY = motionEvent->getY() - m_lastCursorPos.y;

                MouseMoved->emit(deltaX, deltaY);
                
                m_lastCursorPos = { motionEvent->getX(), motionEvent->getY() };
            } else if(auto* closeEvent = event.downCast<wnd::CloseRequestedEvent>(); closeEvent) {
                CloseRequested->emit();
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

        /// @brief Emitted when the window is resized.
        std::shared_ptr<voxels::Emitter<const ResizeEvent&>> WindowResized = std::make_shared<voxels::Emitter<const ResizeEvent&>>();
        /// @brief Emitted when the mouse is moved. ( Provides the delta from the last position. )
        std::shared_ptr<voxels::Emitter<int32, int32>> MouseMoved = std::make_shared<voxels::Emitter<int32, int32>>();
        /// @brief Emitted when the mouse wheel scrolls.
        std::shared_ptr<voxels::Emitter<const ScrollEvent&>> CursorScrolled = std::make_shared<voxels::Emitter<const ScrollEvent&>>();
        /// @brief Emitted when the window has been requested to close.
        std::shared_ptr<voxels::Emitter<>> CloseRequested = std::make_shared<voxels::Emitter<>>();

    private:
        std::bitset<0xFF> m_pressedKeys; //< Bitset for all the pressed keys.
        std::bitset<0xFF> m_releasedKeys; //< Bitset for all the released keys.
        std::bitset<5> m_pressedMouseButtons; //< Bitset for all the pressed mouse buttons.
        std::bitset<5> m_releasedMouseButtons; //< Bitset for all the released mouse buttons.
        ModifierState m_modState; //< The modifier state.
        struct { int32 x, y; } m_lastCursorPos;
    };
}