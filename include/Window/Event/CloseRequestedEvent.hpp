#pragma once

#include "Window/Event/IEvent.hpp"

namespace wnd {
    /**
     * @brief Gets emitted when the user requests to close the window.
     */
    class CloseRequestedEvent : public IEvent {
    public:
        [[nodiscard]] virtual constexpr const char* getName() const noexcept override { return "CloseRequestedEvent"; }  
    };
}