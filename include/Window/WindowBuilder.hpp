#pragma once

#include "Global.hpp"
#include "Window/IWindow.hpp" // For the window
#include <memory> // For smart pointers
#include <string> // For std::string

static constexpr const char* const DEFAULT_WINDOW_TITLE = "Window";     //< Default window title
static constexpr const uint32 DEFAULT_WINDOW_WIDTH = 1280;              //< Default window width
static constexpr const uint32 DEFAULT_WINDOW_HEIGHT = 720;              //< Default window height

namespace wnd {
    /**
     * @brief An os independent window builder.
     */
    class WindowBuilder {
    public:
        /**
         * @brief Construct a new WindowBuilder.
         * 
         * @param title The window title.
         * @param width The window width.
         * @param height The window height.
         */
        [[nodiscard]] explicit WindowBuilder(std::string&& title = DEFAULT_WINDOW_TITLE, uint32 width = DEFAULT_WINDOW_WIDTH, uint32 height = DEFAULT_WINDOW_HEIGHT) noexcept
            : m_title(std::move(title))
            , m_width(width), m_height(height) {  }

        /**
         * @brief Set the dimensions of the window.
         * 
         * @param width The new width.
         * @param height The new height.
         * @return WindowBuilder& A reference to this.
         */
        WindowBuilder& setDimensions(uint32 width, uint32 height) noexcept {
            m_width = width;
            m_height = height;
            return *this;
        }
        /**
         * @brief Set the title of the window.
         * 
         * @param title The new title.
         * @return WindowBuilder& A reference to this.
         */
        WindowBuilder& setTitle(std::string&& title) noexcept {
            m_title = std::move(title);
            return *this;
        }

        /**
         * @brief Build the window.
         * 
         * @return std::unique_ptr<IWindow> A pointer to the window.
         */
        [[nodiscard]] std::unique_ptr<IWindow> build() const;

    private:
        uint32 m_width, m_height; //< Window dimensions.
        std::string m_title; //< Window title.
    };
}