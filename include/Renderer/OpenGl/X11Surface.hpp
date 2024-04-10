#pragma once

#include "Global.hpp"
#ifdef WINDOW_API_X11

#include "Renderer/ISurface.hpp" // For the interface
#include "glad/glx.h" // For the GLX objects.

namespace renderer::opengl {
    /**
     * @brief The opengl X11 surface.
     */
    class X11Surface: public renderer::ISurface {
    public:
        explicit X11Surface(Display* display, GLXDrawable drawable, GLXContext context) noexcept
            : m_x11Display(display), m_x11WindowHandle(drawable), m_context(context), m_viewportSize(1, 1) {  }

        virtual void bind() const noexcept override {
            glXMakeCurrent(m_x11Display, m_x11WindowHandle, m_context);
        }
        virtual void unBind() const noexcept override {
            glXMakeCurrent(m_x11Display, 0, nullptr);
        }
        virtual void swapBuffers() override {
            glXSwapBuffers(m_x11Display, m_x11WindowHandle);
        }
        virtual void setViewportSize(uint32 width, uint32 height) noexcept override {
            bind();
            glViewport(0, 0, width, height);
            m_viewportSize = { width, height };
            unBind();
        }
        [[nodiscard]] virtual std::pair<uint32, uint32> getViewportSize() const noexcept override {
            return m_viewportSize;
        }

    private:
        Display* m_x11Display; //< The display connection.
        GLXDrawable m_x11WindowHandle; //< The window handle.
        GLXContext m_context; //< The OpenGl context.
        std::pair<uint32, uint32> m_viewportSize; //< The current viewport size.
    };
}

#endif