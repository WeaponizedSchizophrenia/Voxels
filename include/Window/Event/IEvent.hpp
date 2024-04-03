#pragma once

#include "Global.hpp"
#include <concepts> // Constrains the downCast method to implement the IEvent interface

namespace wnd {
    /**
     * @brief Generic window event interface.
     */
    class IEvent { 
    public:
        virtual ~IEvent() = default;

        /**
         * @brief Downcasts to the specified event type.
         * 
         * @tparam T The event type to cast to. ( Must implement the IEvent interface. )
         * @return T* The pointer to the casted event.
         */
        template <std::derived_from<IEvent> T>
        [[nodiscard]] inline T* downCast() noexcept {
            return dynamic_cast<T*>(this);
        }

        /**
         * @brief Downcasts to the specified event type.
         * 
         * @tparam T The event type to cast to. ( Must implement the IEvent interface. )
         * @return T* The const pointer to the casted event.
         */
        template<std::derived_from<IEvent> T>
        [[nodiscard]] inline const T* downCast() const noexcept {
            return dynamic_cast<const T*>(this);
        }

        /**
         * @brief Returns the event name.
         * 
         * @return constexpr const char* The event name.
         */
        [[nodiscard]] virtual constexpr const char* getName() const noexcept = 0;
    };

    /**
     * @brief Indicates whether the button is pressed or released.
     */
    enum class ButtonState : uint8 { Pressed = 0, Released = 1 };
}