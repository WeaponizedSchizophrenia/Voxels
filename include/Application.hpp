#pragma once

#include "Observer.hpp"
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
            , m_wndResizeObserver(m_windowManager.getEventParser().WindowResized,
                [this](const wnd::ResizeEvent& event) {
                    m_renderer->getSurface().setViewportSize(event.getWidth(), event.getHeight());
                })
            , m_mouseMoveObserver(m_windowManager.getEventParser().MouseMoved,
                [this](int32 x, int32 y) {
                    static constexpr auto SENSITIVITY = 0.007f;
                    if(m_windowManager.getEventParser().isHeld(wnd::MouseButtonEvent::Button::Middle)) {
                        yaw += static_cast<float32>(x) * SENSITIVITY;
                        pitch += static_cast<float32>(y) * -SENSITIVITY;
                    }
                })
            , m_scrollObserver(m_windowManager.getEventParser().CursorScrolled,
                [this](const wnd::ScrollEvent& event) {
                    fov += static_cast<float32>(event.getDelta()) * -0.01f;
                }) {  }

        void start() noexcept;

    private:
        wnd::WindowManager m_windowManager; //< The window manager of the application.
        std::unique_ptr<renderer::IRenderer> m_renderer; //< The renderer of the application.
        voxels::Observer<const wnd::ResizeEvent&> m_wndResizeObserver; //< Observer for the window resize event.
        voxels::Observer<int32, int32> m_mouseMoveObserver; //< Observer for the mouse move event.
        voxels::Observer<const wnd::ScrollEvent&> m_scrollObserver; //< Observer for the scroll event.
        float32 pitch = 0, yaw = -glm::half_pi<float32>();
        glm::vec3 position = { 0, 0, 5 };
        float32 fov = glm::radians(45.0f);
    };
}