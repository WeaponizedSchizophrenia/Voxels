#include "Application.hpp"

void voxels::Application::start() noexcept {
    m_windowManager.runLoop([&](wnd::LoopDescriptor& loopDescriptor) {

        if(loopDescriptor.eventParser.isPressed(wnd::Key::Escape)) {
            loopDescriptor.shouldClose = true;
        }

        m_cameraController.onUpdate(loopDescriptor.deltaTime);

        m_renderer->render(m_cameraController.getCameraDescriptor());

    });
}