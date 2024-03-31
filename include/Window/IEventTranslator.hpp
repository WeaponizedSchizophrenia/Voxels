#pragma once

#include "Window/Event/IEvent.hpp"
#include <memory> // For smart pointers

namespace wnd {
    /**
     * @brief A generic event translator interface.
     * 
     * @tparam T The native event type to translate from.
     */
    template <typename T>
    class IEventTranslator {
    public:
        virtual ~IEventTranslator() = default;
        /**
         * @brief Translates the native event to a generic IEvent.
         * 
         * @param event The native event.
         * @return std::unique_ptr<IEvent> The generic IEvent.
         */
        virtual std::unique_ptr<IEvent> translateEvent(const T& event) const noexcept = 0;
    };
}