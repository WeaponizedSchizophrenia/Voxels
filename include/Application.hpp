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
                voxels::Observer<const wnd::ResizeEvent&>(m_windowManager.getEventParser().WindowResized,
                    [this](const wnd::ResizeEvent& event) {
                        m_renderer->getSurface().setViewportSize(event.getWidth(), event.getHeight());
                    })
            ) {  }

        void start() noexcept;

    private:
        wnd::WindowManager m_windowManager;
        std::unique_ptr<renderer::IRenderer> m_renderer;
        voxels::Observer<const wnd::ResizeEvent&> m_wndResizeObserver;
    };
}