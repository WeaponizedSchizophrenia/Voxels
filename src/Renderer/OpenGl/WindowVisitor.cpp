/**
 * @file WindowVisitor.cpp
 * @author WeaponizedSchizophrenia (you@domain.com)
 * @brief This file contains the source for the WindowVisitor class.
 */

#include "Renderer/OpenGl/WindowVisitor.hpp" // For declarations.
#include "Exception.hpp" // For exceptions.
#include "Renderer/ISurface.hpp" // For the surface.
#include "Renderer/OpenGl/X11Surface.hpp" // For the x11 surface implementation.
#include "Window/Linux/X11/Window.hpp" // For x11 window implementation.
#include "glad/glad_glx.h" // For GLX functions.
#include <memory> // For smart pointers.

/// @brief The OpenGl attributes.
static GLint ATTRIBUTES[] = {
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER,
    GLX_RGBA,
    None
};

std::unique_ptr<renderer::ISurface> renderer::opengl::WindowVisitor::visit(const wnd::IWindow& window) const {
    THROW_EXCEPTION("The window api is not supported by OpenGL implementation.");
}

std::unique_ptr<renderer::ISurface> renderer::opengl::WindowVisitor::visit(const wnd::x11::Window& window) const {
    auto display = window.getDisplay();
    auto screen = XDefaultScreen(display);
    if(!gladLoadGLX(display, screen)) {
        THROW_EXCEPTION("Could not load GLX.");
    }

    auto visual = glXChooseVisual(display, screen, ATTRIBUTES);
    auto ctx = glXCreateContext(display, visual, nullptr, GL_TRUE);

    return std::make_unique<renderer::opengl::X11Surface>(display, window.getWindowId(), ctx);
}