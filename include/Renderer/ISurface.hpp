#pragma once

#include "Global.hpp"
#include "Renderer/IBindable.hpp" // For the interface.
#include <utility>

namespace renderer {
    /**
     * @brief Interface for a surface.
     */
    class ISurface: public IBindable {
    public:
        using IBindable::bind, IBindable::unBind;

        virtual ~ISurface() = default;
        /**
         * @brief Swaps the surface's buffers.
         */
        virtual void swapBuffers() = 0;
        /**
         * @brief Updates the viewport of the surface.
         * 
         * @param width The new width.
         * @param height The new height.
         */
        virtual void setViewportSize(uint32 width, uint32 height) noexcept = 0;
        /**
         * @brief Gets the viewport size.
         * 
         * @return std::pair<uint32, uint32> The width and height.
         */
        [[nodiscard]] virtual std::pair<uint32, uint32> getViewportSize() const noexcept = 0;
    };
}