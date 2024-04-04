#pragma once

#include "Global.hpp"
#include "Observer.hpp"
#include "Window/IWindow.hpp"
#include "Window/EventParser.hpp"
#include <functional>
#include <memory>

namespace wnd {
    struct LoopDescriptor {
        bool m_shouldClose = false;
        const float64 m_deltaTime;
        const EventParser& m_inputParser;
    };

    class WindowManager {
    public:
        /**
         * @brief A callback that gets called for each loop iteration.
         * 
         * @param loopDescriptor The loop descriptor.
         */
        using LoopCallback = std::function<void(LoopDescriptor& loopDescriptor)>;

        explicit WindowManager(std::unique_ptr<IWindow>&& window) noexcept;

        /**
         * @brief Runs the loop.
         * 
         * @param callback The callback to call.
         */
        void runLoop(const LoopCallback& callback);

        /**
         * @brief Gets the event parser.
         * 
         * @return const EventParser& A reference to the event parser.
         */
        [[nodiscard]] inline const EventParser& getEventParser() const noexcept { return m_eventParser; }

    private:
        std::unique_ptr<IWindow> m_window; //< The managed window.
        EventParser m_eventParser; //< The input parser. 
        voxels::Observer<> m_closeRequestedObserver; //< Observer for the close requested event.
    };
}