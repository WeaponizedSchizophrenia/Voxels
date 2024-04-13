#pragma once

#include "CameraDescriptor.hpp"
#include "Renderer/ISurface.hpp"

namespace renderer {
    class IRenderer {
    public:
        virtual ~IRenderer() = default;


        /**
         * @brief Gets the Surface.
         * 
         * @return ISurface& A mutable reference to the surface.
         */
        [[nodiscard]] virtual ISurface& getSurface() noexcept = 0;
        /**
         * @brief Gets the Surface.
         * 
         * @return const ISurface& A const reference to the surface.
         */
        [[nodiscard]] virtual const ISurface& getSurface() const noexcept = 0;

        /**
         * @brief Renders the held data.
         *
         * @param cameraDescriptor The camera descriptor.
         */
        virtual void render(const voxels::CameraDescriptor& cameraDescriptor) = 0;
    };
}