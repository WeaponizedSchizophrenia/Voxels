#pragma once

#include "Renderer/ISurface.hpp" // For the return values.
#include <memory> // For the smart pointers.

namespace wnd { class IWindow; }
namespace wnd::x11 { class Window; }

namespace renderer {
    /**
     * @brief Abstract window visitor.
     */
    class IWindowVisitor {
    public:
        [[nodiscard]] virtual std::unique_ptr<ISurface> visit(const wnd::IWindow& window) const = 0;
        [[nodiscard]] virtual std::unique_ptr<ISurface> visit(const wnd::x11::Window& window) const = 0;
    };
}