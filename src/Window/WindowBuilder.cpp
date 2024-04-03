/**
 * @file WindowBuilder.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the WindowBuilder class.
 */

#include "Window/WindowBuilder.hpp" // Declarations
#include "Window/Linux/X11/Window.hpp" // The window to use for linux
#include "Window/Linux/X11/EventTranslator.hpp" // The default event traslator to use for x11

std::unique_ptr<wnd::IWindow> wnd::WindowBuilder::build() const {
    /* TODO: Add support for windows. */
    #ifdef WINDOW_API_X11
    // Just create the x11 window with the held properties.
    return std::make_unique<x11::Window>(m_title, m_width, m_height, std::make_unique<x11::EventTranslator>());;
    #else
    #error "Unsupported platform"
    #endif
}