#pragma once

#include "Renderer/IRenderer.hpp" // For interface.
#include "Renderer/ISurface.hpp" // For the surface
#include "Window/IWindow.hpp" // For the window
#include <memory> // For smart pointers

namespace renderer::opengl {
    /**
     * @brief OpenGl renderer.
     */
    class Renderer: public IRenderer {
    public:
        /**
         * @brief Construct a new OpenGL renderer.
         * 
         * @param window The window on to which to render.
         *
         * @throws voxels::Exception If the window api is not supported.
         */
        explicit Renderer(const wnd::IWindow& window);

        [[nodiscard]] virtual inline ISurface& getSurface() noexcept override { return *m_surface; }
        [[nodiscard]] virtual inline const ISurface& getSurface() const noexcept override { return *m_surface; }

        virtual void render() override;

    private:
        std::shared_ptr<ISurface> m_surface; //< The surface to render to.
    };
}