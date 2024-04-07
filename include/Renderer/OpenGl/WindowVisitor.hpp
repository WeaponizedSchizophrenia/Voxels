#pragma once

#include "Renderer/IWindowVisitor.hpp" // For the interface.

namespace renderer::opengl {
    /**
     * @brief Window visitor that creates an OpenGl surface.
     */
    class WindowVisitor: public renderer::IWindowVisitor {
    public:
        [[nodiscard]] virtual std::unique_ptr<renderer::ISurface> visit(const wnd::IWindow& window) const override;
        [[nodiscard]] virtual std::unique_ptr<renderer::ISurface> visit(const wnd::x11::Window& window) const override;
    };
}