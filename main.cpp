#include "Renderer/Vulkan/Renderer.hpp"
#include "Window/Event/CloseRequestedEvent.hpp"
#include "Window/Event/IEvent.hpp"
#include "Window/Event/KeyboardEvent.hpp"
#include "Window/Event/MouseButtonEvent.hpp"
#include "Window/Event/MouseMotionEvent.hpp"
#include "Window/Event/ResizeEvent.hpp"
#include "Window/Event/ScrollEvent.hpp"
#include "Window/IWindow.hpp"
#include "Window/WindowBuilder.hpp"

#include <iostream>
#include <memory>

int main() {
    /* TEMPORARY FOR TESTING */
    try {
        auto vRenderer = vulkan::Renderer("Voxels");

        std::cout << "Renderer did't throw an exception.\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    auto window = wnd::WindowBuilder("Voxels")
        .setDimensions(800, 600)
        .build();

    /* TEMPORARY FOR TESTING */
    window->runLoop([&](wnd::IEvent* event) {
        if(event) {
            if(auto* mbEvent = event->downCast<wnd::MouseButtonEvent>(); mbEvent) {
                if(mbEvent->isPressed())
                    std::cout << mbEvent->getName() << " (pressed) : " << (int)mbEvent->getButton() << '\n';
                else
                    std::cout << mbEvent->getName() << " (released) : " << (int)mbEvent->getButton() << '\n';
            } else if(auto* motionEvent = event->downCast<wnd::MouseMotionEvent>(); motionEvent) {
                std::cout << motionEvent->getName() << " : " << motionEvent->getX() << ", " << motionEvent->getY() << '\n';
            } else if(auto* closeEvent = event->downCast<wnd::CloseRequestedEvent>(); closeEvent) {
                std::cout << closeEvent->getName() << '\n';
                window->close();
            } else if(auto* resizeEvent = event->downCast<wnd::ResizeEvent>(); resizeEvent) {
                std::cout << resizeEvent->getName() << " : " << resizeEvent->getWidth() << ", " << resizeEvent->getHeight() << '\n';
            } else if(auto* keyEvent = event->downCast<wnd::KeyboardEvent>(); keyEvent) {
                if(keyEvent->isPressed())
                    std::cout << keyEvent->getName() << " (pressed) : 0x" << std::hex << (int)keyEvent->getKey() << std::dec << '\n';
                else
                    std::cout << keyEvent->getName() << " (released) : 0x" << std::hex << (int)keyEvent->getKey() << std::dec << '\n';
                if(keyEvent->isPressed() && keyEvent->getKey() == wnd::Key::Escape)
                    window->close();
                if(keyEvent->isPressed() && keyEvent->getKey() == wnd::Key::Space)
                    window->setVisibility(false);
            } else if(auto* scrollEvent = event->downCast<wnd::ScrollEvent>(); scrollEvent) {
                std::cout << scrollEvent->getName() << " : " << static_cast<int32>(scrollEvent->getDelta()) << '\n';
            }
        }
    });

    return 0;
}
