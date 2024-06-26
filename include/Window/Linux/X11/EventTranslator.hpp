#pragma once

#include "Global.hpp"

#ifdef WINDOW_API_X11

#include "Window/IEventTranslator.hpp" // For the interface.
#include <X11/Xlib.h> // X11 library for the delete window protocol.

namespace wnd::x11 {
    /**
     * @brief Default X11 event translator.
     */
    class EventTranslator : public wnd::IEventTranslator<XEvent> {
    public:
        [[nodiscard]] EventTranslator() noexcept : m_deleteWindowProtocol(XInternAtom(XOpenDisplay(nullptr), "WM_DELETE_WINDOW", false)) { }

        [[nodiscard]] std::unique_ptr<wnd::IEvent> translateEvent(const XEvent& event) const noexcept override;

    private:
        Atom m_deleteWindowProtocol; //< Atom for the WM_DELETE_WINDOW protocol.
    };
}
#endif