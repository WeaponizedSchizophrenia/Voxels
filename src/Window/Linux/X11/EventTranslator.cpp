/**
 * @file EventTranslator.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the default X11 event translator.
 */

#include "Window/Linux/X11/EventTranslator.hpp" // Declarations
#include "Window/Event/KeyboardEvent.hpp" // Keyboard event
#include "Window/Event/MouseButtonEvent.hpp" // Mouse button event
#include "Window/Event/MouseMotionEvent.hpp" // Mouse motion event
#include "Window/Event/ResizeEvent.hpp" // Resize event
#include "Window/Event/ScrollEvent.hpp" // Scroll event
#include "Window/Event/CloseRequestedEvent.hpp" // Close requested event

std::unique_ptr<wnd::IEvent> x11::EventTranslator::translateEvent(const XEvent& event) const noexcept {
    switch (event.type) {
        case ButtonPress: {
            switch (event.xbutton.button) {
                // Buttons 4 and 5 are mapped to scrolling.
                case 4: return std::make_unique<wnd::ScrollEvent>(1);
                case 5: return std::make_unique<wnd::ScrollEvent>(-1);
                // Buttons 8 and 9 are mapped to side buttons.
                case 8: return std::make_unique<wnd::MouseButtonEvent>(wnd::MouseButtonEvent::Button::Side1, wnd::ButtonState::Pressed);
                case 9: return std::make_unique<wnd::MouseButtonEvent>(wnd::MouseButtonEvent::Button::Side2, wnd::ButtonState::Pressed);
                // The rest map to the enum.
                default: return std::make_unique<wnd::MouseButtonEvent>(static_cast<wnd::MouseButtonEvent::Button>(event.xbutton.button), wnd::ButtonState::Pressed);
            }
        }
        case ButtonRelease: {
            switch (event.xbutton.button) {
                // Buttons 4 and 5 are mapped to scrolling, but we don't need to create a scroll event on release.
                case 4: break;
                case 5: break;
                // The rest is the same as in the ButtonPress case except with a different ButtonState.
                case 8: return std::make_unique<wnd::MouseButtonEvent>(wnd::MouseButtonEvent::Button::Side1, wnd::ButtonState::Released);
                case 9: return std::make_unique<wnd::MouseButtonEvent>(wnd::MouseButtonEvent::Button::Side2, wnd::ButtonState::Released);
                default: return std::make_unique<wnd::MouseButtonEvent>(static_cast<wnd::MouseButtonEvent::Button>(event.xbutton.button), wnd::ButtonState::Released);
            }
        }
        case MotionNotify: {
            // Cursor movement event.
            return std::make_unique<wnd::MouseMotionEvent>(event.xmotion.x, event.xmotion.y);
        }
        case KeyPress: {
            // Get the system key which maps to the Key enum.
            auto key = XLookupKeysym(const_cast<XKeyEvent*>(&event.xkey), 0);
            return std::make_unique<wnd::KeyboardEvent>(static_cast<wnd::Key>(key), wnd::ButtonState::Pressed);
        }
        case KeyRelease: {
            // Same as the KeyPress case except for the different ButtonState.
            auto key = XLookupKeysym(const_cast<XKeyEvent*>(&event.xkey), 0);
            return std::make_unique<wnd::KeyboardEvent>(static_cast<wnd::Key>(key), wnd::ButtonState::Released);
        }
        case ClientMessage: {
            // This if statement checks if the user has requested to close the window.
            if(event.xclient.data.l[0] == m_deleteWindowProtocol)
                return std::make_unique<wnd::CloseRequestedEvent>();
        }
        case Expose: {
            // Resize event.
            return std::make_unique<wnd::ResizeEvent>(event.xexpose.width, event.xexpose.height);
        }
    }
    return nullptr;
}