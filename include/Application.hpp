#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/OpenGl/Renderer.hpp"
#include "Window/WindowBuilder.hpp"
#include "Window/WindowManager.hpp"
#include <memory>

namespace voxels {
    class Application {
    public:
        explicit Application()
            : m_windowManager(wnd::WindowBuilder()
                .setTitle("Voxels")
                .buildManager())
            , m_renderer(std::make_unique<renderer::opengl::Renderer>(m_windowManager.getWindow()))
            , m_wndResizeObserver(
                // Resize the renderer's surface when the window is resized.
                voxels::Observer<const wnd::ResizeEvent&>(m_windowManager.getEventParser().WindowResized,
                    [this](const wnd::ResizeEvent& event) {
                        m_renderer->getSurface().setViewportSize(event.getWidth(), event.getHeight());
                    })
            ) {  }

        void start() noexcept;

    private:
        wnd::WindowManager m_windowManager; //< The window manager of the application.
        std::unique_ptr<renderer::IRenderer> m_renderer; //< The renderer of the application.
        voxels::Observer<const wnd::ResizeEvent&> m_wndResizeObserver; //< Observer for the window resize event.
    };
}