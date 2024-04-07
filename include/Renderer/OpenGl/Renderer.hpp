#pragma once

#include "Renderer/ISurface.hpp" // For the surface
#include "Window/IWindow.hpp" // For the window
#include <memory> // For smart pointers

namespace renderer::opengl {
    /**
     * @brief OpenGl renderer.
     */
    class Renderer {
    public:
        explicit Renderer(const wnd::IWindow& window);

        /**
         * @brief Renders the held data.
         */
        void render(int32 width, int32 height);

    private:
        std::unique_ptr<renderer::ISurface> m_surface; //< The surface to render to.
    };
}