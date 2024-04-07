#pragma once

#include "Global.hpp"
#include "Renderer/IWindowVisitor.hpp" // For the visitor
#include "Renderer/ISurface.hpp" // For the context return from visitor
#include "Window/Event/IEvent.hpp" // For events
#include <functional> // For the OnLoopIterationCallback
#include <memory> // For smart pointers

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
         * @param deltaTime The time between this frame and the last in seconds.
         */
        using OnLoopIterationCallback = std::function<void(IEvent* event, float64 deltaTime)>;

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
        [[nodiscard]] virtual bool getVisibility() const noexcept = 0;
        /**
         * @brief Runs the window loop.
         * 
         * @param callback The callback that gets called for each loop iteration.
         *
         * @throws Only if the provided callback throws.
         */
        virtual void runLoop(const OnLoopIterationCallback& callback) = 0;

        /**
         * @brief Accepts a window visitor and returns the visitor returned IContext.
         * 
         * @param visitor The window visitor
         * @return std::unique_ptr<renderer::IContext> The visitor returned IContext.
         */
        [[nodiscard]] virtual std::unique_ptr<renderer::ISurface> acceptVisitor(const renderer::IWindowVisitor& visitor) const {
            return visitor.visit(*this);
        }
    };
}