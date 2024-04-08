#pragma once

#include "Renderer/IWindowVisitor.hpp" // For the interface.

namespace renderer::opengl {
    /**
     * @brief Window visitor that creates an OpenGl surface.
     */
    class WindowVisitor: public renderer::IWindowVisitor {
    public:
        /**
         * @brief Visits a generic IWindow.
         * 
         * @param window The generic IWindow.
         *
         * @note It does not return anything because it always throws because it can not create an OpenGl surface from a generic window.
         *
         * @throws voxels::Exception Every single time because it can not create an OpenGl surface from a generic window.
         */
        [[nodiscard]] [[noreturn]] virtual std::unique_ptr<renderer::ISurface> visit(const wnd::IWindow& window) const override;
        [[nodiscard]] virtual std::unique_ptr<renderer::ISurface> visit(const wnd::x11::Window& window) const override;
    };
}