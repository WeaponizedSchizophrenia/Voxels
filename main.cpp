#include "Global.hpp"
#include "Observer.hpp"
#include "Renderer/OpenGl/Renderer.hpp"
#include "Window/Event/ResizeEvent.hpp"
#include "Window/WindowBuilder.hpp"
#include "Window/WindowManager.hpp"
#include <iostream>

int main() {
    auto windowManager = wnd::WindowBuilder("Voxels")
        .setDimensions(800, 600)
        .buildManager();

    auto renderer = renderer::opengl::Renderer(windowManager.getWindow());
    auto observer = voxels::Observer<const wnd::ResizeEvent&>(windowManager.getEventParser().WindowResized, [&](const wnd::ResizeEvent& event) {
        renderer.getSurface().updateViewport(event.getWidth(), event.getHeight());
    });

    windowManager.runLoop([&](wnd::LoopDescriptor& loopDescriptor) {

        if(loopDescriptor.m_inputParser.isPressed(wnd::Key::Escape)) {
            loopDescriptor.m_shouldClose = true;
        }
        if(loopDescriptor.m_inputParser.isPressed(wnd::Key::Function1)) {
            std::cout << loopDescriptor.m_deltaTime << '\n';
        }

        renderer.render();

    });

    return 0;
}
