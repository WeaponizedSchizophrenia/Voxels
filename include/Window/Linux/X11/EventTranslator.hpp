#pragma once
#ifdef __linux__

#include "Window/IEventTranslator.hpp"
#include <X11/Xlib.h>

namespace x11 {
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