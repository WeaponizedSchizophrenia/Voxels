#include "Window/WindowBuilder.hpp"
#include "Window/WindowManager.hpp"
#include <iostream>

int main() {
    auto windowManager = wnd::WindowBuilder("Voxels")
        .setDimensions(800, 600)
        .buildManager();

    windowManager.runLoop([](wnd::LoopDescriptor& loopDescriptor) {
        if(loopDescriptor.m_inputParser.isPressed(wnd::Key::Escape)) {
            loopDescriptor.m_shouldClose = true;
        }
        if(loopDescriptor.m_inputParser.isPressed(wnd::Key::Function1)) {
            std::cout << loopDescriptor.m_deltaTime << '\n';
        }
    });

    return 0;
}
