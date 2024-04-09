#pragma once

#include "Global.hpp"
#include "Renderer/IBindable.hpp" // For the interface.

namespace renderer {
    /**
     * @brief Interface for a surface.
     */
    class ISurface: public IBindable {
    public:
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
        virtual void updateViewport(uint32 width, uint32 height) = 0;
        virtual void bind() const noexcept override = 0;
        virtual void unBind() const noexcept override = 0;
    };
}