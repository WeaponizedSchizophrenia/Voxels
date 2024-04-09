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
        /**
         * @brief Construct a new OpenGL renderer.
         * 
         * @param window The window on to which to render.
         *
         * @throws voxels::Exception If the window api is not supported.
         */
        explicit Renderer(const wnd::IWindow& window);

        /**
         * @brief Renders the held data.
         */
        void render();

        /**
         * @brief Gets the Surface.
         * 
         * @return const ISurface& A const reference to the surface.
         */
        [[nodiscard]] inline const ISurface& getSurface() const noexcept { return *m_surface; }
        /**
         * @brief Gets the Surface.
         * 
         * @return ISurface& A mutable reference to the surface.
         */
        [[nodiscard]] inline ISurface& getSurface() noexcept { return *m_surface; }

    private:
        std::shared_ptr<ISurface> m_surface; //< The surface to render to.
    };
}