#include "Exception.hpp"
#include "Window/Event/CloseRequestedEvent.hpp"
#include "Window/Event/IEvent.hpp"
#include "Window/Event/Key.hpp"
#include "Window/Event/MouseButtonEvent.hpp"
#include "Window/InputEventParser.hpp"
#include "Window/IWindow.hpp"
#include "Window/InputEventParser.hpp"
#include "Window/WindowBuilder.hpp"

#include <iostream>
#include <memory>

int main() {
    auto window = wnd::WindowBuilder("Voxels")
        .setDimensions(800, 600)
        .build();

    auto inputParser = wnd::InputEventParser();

    /* TEMPORARY FOR TESTING */
    window->runLoop([&](wnd::IEvent* event) {

        if(event) {

            inputParser.parseEvent(*event);

            if(inputParser.isPressed(wnd::Key::Escape)) {
                window->close();
            }
            if(inputParser.isPressed(wnd::Key::LeftControl)) {
                std::cout << "pressed ctrl\n";
            }
            if(inputParser.isReleased(wnd::Key::LeftControl)) {
                std::cout << "released ctrl\n";
            }
            if(inputParser.isPressed(wnd::Key::Space)) {
                std::cout << "pressed space\n";
            }
            if(inputParser.isReleased(wnd::Key::Space)) {
                std::cout << "released space\n";
            }
            if(inputParser.isPressed(wnd::MouseButtonEvent::Button::Side1)
                && inputParser.getModifierState().m_leftCtrl) {
                std::cout << "Side1 + lCtrl\n";
            }

            if(auto* closeRequested = event->downCast<wnd::CloseRequestedEvent>(); closeRequested) {
                window->close();
            }

        } 

        inputParser.onLoopEnded();

    });

    return 0;
}
