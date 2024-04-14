#pragma once

#include "Observer.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/OpenGl/Renderer.hpp"
#include "Scripts/CameraController.hpp"
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
            , m_wndResizeObserver(m_windowManager.getEventParser()->WindowResized,
                [this](const wnd::ResizeEvent& event) {
                    m_renderer->getSurface().setViewportSize(event.getWidth(), event.getHeight());
            })
            , m_cameraController(
                world::Camera(glm::vec3{ 0.0f, 0.0f, 5.0f}),
                m_windowManager.getEventParser()
            ) {  }

        void start() noexcept;

    private:
        wnd::WindowManager m_windowManager; //< The window manager of the application.
        std::unique_ptr<renderer::IRenderer> m_renderer; //< The renderer of the application.
        voxels::Observer<const wnd::ResizeEvent&> m_wndResizeObserver; //< Observer for the window resize event.
        scripts::CameraController m_cameraController; //< The camera of the application.
    };
}