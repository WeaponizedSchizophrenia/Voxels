/**
 * @file Window.cpp
 * @author WeaponizedSchizophrenia
 * @brief This file contains the source for the X11 Window class.
 */

#include "Global.hpp"
#ifdef WINDOW_API_X11
#include "Window/Linux/X11/Window.hpp" // Declarations.
#include "Exception.hpp" // For exceptions.
#include <X11/X.h> // For X11.
#include <X11/Xlib.h> // For X11.
#include <X11/Xutil.h> // X11 utility.
#include <chrono> // Required for the delta time calculations.


wnd::x11::Window::Window(std::string_view title, uint32 width, uint32 height, std::unique_ptr<wnd::IEventTranslator<XEvent>>&& eventTranslator)
    : m_x11Display(XOpenDisplay(nullptr))
    , m_eventTranslator(std::move(eventTranslator))
    , m_width(width), m_height(height)
{
    // Throw if the display could not be opened.
    if(!m_x11Display) {
        THROW_EXCEPTION("Failed to open X11 display.");
    }

    // Set the window attributes.
    XSetWindowAttributes winAttributes;
    winAttributes.event_mask = ExposureMask
        | KeyPressMask | KeyReleaseMask 
        | ButtonPressMask | ButtonReleaseMask
        | PointerMotionMask;
    // Create the window.
    m_x11WindowId = XCreateWindow(
        m_x11Display,
        XRootWindowOfScreen(XDefaultScreenOfDisplay(m_x11Display)),
        100, 50,
        width, height,
        15,
        CopyFromParent,
        InputOutput,
        CopyFromParent,
        CWEventMask,
        &winAttributes
    );

    // Set the size hints.
    XSizeHints sizeHints;
    sizeHints.flags = PSize;
    sizeHints.width = width;
    sizeHints.height = height;
    // Set the window properties.
    XSetStandardProperties(
        m_x11Display,
        m_x11WindowId,
        title.cbegin(),
        title.cbegin(),
        None,
        nullptr,
        0,
        &sizeHints
    );

    // Set the WM_DELETE_WINDOW protocol so that the event system gets notified about a close request.
    auto wmDeleteProtocol = XInternAtom(m_x11Display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(m_x11Display, m_x11WindowId, &wmDeleteProtocol, 1);
}

wnd::x11::Window::~Window() {
    // Unmap the window before destroying it just in case.
    XUnmapWindow(m_x11Display, m_x11WindowId);
    // Destroy the window.
    XDestroyWindow(m_x11Display, m_x11WindowId);
    m_x11WindowId = 0;

    // Close the display connection.
    XCloseDisplay(m_x11Display);
    m_x11Display = nullptr;
}

void wnd::x11::Window::runLoop(const OnLoopIterationCallback& callback) {
    // Map the window.
    XMapWindow(m_x11Display, m_x11WindowId);

    // Store the last frame start time.
    auto lastFrameStart = std::chrono::steady_clock::now();
    while(m_open) {
        // Calculate the delta time.
        auto frameStart = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration<float64>(frameStart - lastFrameStart).count();
        // Set the last frame start time.
        lastFrameStart = frameStart;

        XEvent event;
        std::vector<std::unique_ptr<wnd::IEvent>> events;
        events.reserve(XEventsQueued(m_x11Display, QueuedAlready));
        // If there are events in the queue, get all of them.
        while(XEventsQueued(m_x11Display, QueuedAlready) > 0) {
            XNextEvent(m_x11Display, &event);
            // Translate the event with the provided translator.
            events.push_back(m_eventTranslator->translateEvent(event));
        }

        // Clear the window.
        XClearWindow(m_x11Display, m_x11WindowId);

        // Call the provided callback.
        callback(events, deltaTime);
    }
}

void wnd::x11::Window::updateVisibility() noexcept {
    // If the window is visible, raise it, if not, lower it.
    if(m_visible) {
        XRaiseWindow(m_x11Display, m_x11WindowId);
    } else {
        XLowerWindow(m_x11Display, m_x11WindowId);
    }
}

#endif