#include "Application.hpp"

void voxels::Application::start() noexcept {
    m_windowManager.runLoop([&](wnd::LoopDescriptor& loopDescriptor) {

        if(loopDescriptor.m_inputParser.isPressed(wnd::Key::Escape)) {
            loopDescriptor.m_shouldClose = true;
        }

        m_renderer->render();

    });
}