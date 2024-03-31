#pragma once

#include "Window/Event/IEvent.hpp"
#include <functional> // For the OnLoopIterationCallback

namespace wnd {
    /**
     * @brief Generic window interface.
     */
    class IWindow {
    public:
        /**
         * @brief A callback that gets called for each loop iteration.
         * 
         * @param event The event pointer, if it is nullptr that means that no event has occurred.
         */
        using OnLoopIterationCallback = std::function<void(IEvent* event)>;

        virtual ~IWindow() = default;

        /**
         * @brief Closes the window.
         */
        virtual void close() noexcept = 0;
        /**
         * @brief Set the visibility of the window.
         * 
         * @param visible The new window visibility.
         */
        virtual void setVisibility(bool visible) noexcept = 0;
        /**
         * @brief Returns the current window visibility.
         * 
         * @return true If the window is not minimized.
         * @return false If the window is minimized.
         */
        virtual bool getVisibility() const noexcept = 0;
        /**
         * @brief Runs the window loop.
         * 
         * @param callback The callback that gets called for each loop iteration.
         *
         * @throws Only if the provided callback throws.
         */
        virtual void runLoop(const OnLoopIterationCallback& callback) = 0;
    };
}