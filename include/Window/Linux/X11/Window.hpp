#pragma once

#include "Global.hpp"

#ifdef WINDOW_API_X11

#include "Window/IEventTranslator.hpp" // For translating events.
#include "Window/IWindow.hpp" // For the interface.

#include <X11/Xlib.h> // For X11.
#include <memory> // For smart pointers.

namespace wnd::x11 {
    /**
     * @brief X11 Window manager.
     */
    class Window : public wnd::IWindow {
    public:
        [[nodiscard]] explicit Window(std::string_view title, uint32 width, uint32 height, 
            std::unique_ptr<wnd::IEventTranslator<XEvent>>&& eventTranslator);
        virtual ~Window() override;

        Window() = delete;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        virtual void runLoop(const OnLoopIterationCallback& callback) override;

        virtual inline void setVisibility(bool visible) noexcept override {
            m_visible = visible;
            updateVisibility();
        }
        [[nodiscard]] virtual inline bool getVisibility() const noexcept override { return m_visible; }
        virtual inline void close() noexcept override { m_open = false; }

        [[nodiscard]] virtual std::unique_ptr<renderer::ISurface> acceptVisitor(const renderer::IWindowVisitor& visitor) const override {
            return visitor.visit(*this);
        }

        /**
         * @brief Gets the display connection.
         * 
         * @return Display* The x11 display connection.
         */
        [[nodiscard]] Display* getDisplay() const noexcept { return m_x11Display; }
        /**
         * @brief Gets the window id.
         * 
         * @return uint64 The window id.
         */
        [[nodiscard]] uint64 getWindowId() const noexcept { return m_x11WindowId; }

    private:
        /**
         * @brief Updates the window visibility by either minimizing or restoring it.
         */
        void updateVisibility() noexcept;

        Display* m_x11Display; //< Connection to the x11 server.
        uint64 m_x11WindowId; //< Current window id.
        bool m_visible = false; //< Current window visibility.
        bool m_open = true; //< Whether the window is open.
        uint32 m_width, m_height; //< Window dimensions.
        std::unique_ptr<wnd::IEventTranslator<XEvent>> m_eventTranslator; //< Generic event translator.
    };
}

#endif