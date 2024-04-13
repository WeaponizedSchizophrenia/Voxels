#include "Application.hpp"
#include "CameraDescriptor.hpp"

void voxels::Application::start() noexcept {
    m_windowManager.runLoop([&](wnd::LoopDescriptor& loopDescriptor) {

        if(loopDescriptor.m_inputParser.isPressed(wnd::Key::Escape)) {
            loopDescriptor.m_shouldClose = true;
        }

        static constexpr auto SPEED = 5.0f;

        auto deltaPos = glm::vec3{
            loopDescriptor.m_inputParser.isHeld(wnd::Key::KeyA)
            ? -1.0f 
            : loopDescriptor.m_inputParser.isHeld(wnd::Key::KeyD) 
                ? 1.0f : 0.0f,
            loopDescriptor.m_inputParser.isHeld(wnd::Key::KeyE)
            ? -1.0f 
            : loopDescriptor.m_inputParser.isHeld(wnd::Key::KeyQ) 
                ? 1.0f : 0.0f,
            loopDescriptor.m_inputParser.isHeld(wnd::Key::KeyS)
            ? -1.0f
            : loopDescriptor.m_inputParser.isHeld(wnd::Key::KeyW)
                ? 1.0f : 0.0f
        } * SPEED * static_cast<float32>(loopDescriptor.m_deltaTime);


        auto camDescriptor = voxels::CameraDescriptor(position, pitch, yaw, fov);
        deltaPos = deltaPos * camDescriptor.constructViewDirectionMatrix();
        position += deltaPos;
        camDescriptor.position = position;

        m_renderer->render(camDescriptor);

    });
}