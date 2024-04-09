#pragma once

#include "Global.hpp"
#ifdef WINDOW_API_X11

#include "Renderer/ISurface.hpp" // For the interface
#include "glad/glad_glx.h" // For the GLX objects.

namespace renderer::opengl {
    /**
     * @brief The opengl X11 surface.
     */
    class X11Surface: public renderer::ISurface {
    public:
        explicit X11Surface(Display* display, GLXDrawable drawable, GLXContext context) noexcept
            : m_x11Display(display), m_x11WindowHandle(drawable), m_context(context) {  }

        virtual void bind() const noexcept override {
            glXMakeCurrent(m_x11Display, m_x11WindowHandle, m_context);
        }
        virtual void unBind() const noexcept override {
            glXMakeCurrent(m_x11Display, 0, nullptr);
        }
        virtual void swapBuffers() override {
            glXSwapBuffers(m_x11Display, m_x11WindowHandle);
        }
        virtual void updateViewport(uint32 width, uint32 height) override {
            glViewport(0, 0, width, height);
        }

    private:
        Display* m_x11Display; //< The display connection.
        GLXDrawable m_x11WindowHandle; //< The window handle.
        GLXContext m_context; //< The OpenGl context.
    };
}

#endif