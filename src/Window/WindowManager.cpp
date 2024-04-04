#include "Window/WindowManager.hpp"

wnd::WindowManager::WindowManager(std::unique_ptr<IWindow>&& window) noexcept
    : m_window(std::move(window))
    , m_eventParser()
    , m_closeRequestedObserver(
        m_eventParser.CloseRequested,
        [this] {
            m_window->close();
        }
    ) {  }

void wnd::WindowManager::runLoop(const LoopCallback& callback) {
    m_window->runLoop([this, &callback](IEvent* event, float64 deltaTime) {

    // Setup the loop descriptor.
    auto loopDescriptor = wnd::LoopDescriptor {
        false,
        deltaTime,
        m_eventParser
    };

    // Parse the event if there is one.
    if(event) {
        m_eventParser.parseEvent(*event);
    }

    // Call the callback.
    callback(loopDescriptor); 

    // Update the input parser.
    m_eventParser.onLoopEnded();

    // Close the window if it should be closed.
    if(loopDescriptor.m_shouldClose) {
        m_window->close();
    }

});
}