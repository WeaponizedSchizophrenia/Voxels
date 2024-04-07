#pragma once

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
        virtual void bind() const override = 0;
        virtual void unBind() const override = 0;
    };
}