#pragma once

#include "Renderer/IBindable.hpp"
#include "Renderer/OpenGl/Buffer.hpp"
#include "Renderer/OpenGl/Common.hpp"
#include "Renderer/TextureFilter.hpp"
#include "Renderer/TextureFormat.hpp"
#include "Renderer/TextureWrap.hpp"
#include <cstddef>

namespace renderer::opengl {
    struct TextureData {
        void* data;
        uint32 width, height;
    };


    template<size_t D, typename T, TextureFormat F>
    class Texture: public IBindable {
        static_assert(false, "Unsupported texture dimension.");
    };
    template<typename T, TextureFormat F>
    class Texture<2, T, F>: public IBindable {
    public:
        ~Texture() noexcept {
            unBind();
            glDeleteTextures(1, &m_texture);
        }

        virtual void bind() const noexcept override {
            glBindTexture(GL_TEXTURE_2D, m_texture);
        }
        virtual void unBind() const noexcept override {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void copyDataIntoTexture(TextureData data) {
            bind();
            constexpr auto formatDesc = fromTextureFormat(F);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                formatDesc.internal,
                data.width, data.height,
                0,
                formatDesc.format,
                formatDesc.type,
                data.data
            );
            glGenerateMipmap(GL_TEXTURE_2D);
            unBind();
        }

        template<TextureWrap Wrap, TextureFilter Filter>
        [[nodiscard]] static inline Texture createTexture(std::optional<TextureData> data = {}) {
            auto tex = Texture();
            tex.bind();
            auto wrap = fromTextureWrap(Wrap);
            auto filter = fromTextureFilter(Filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            if(data) {
                constexpr auto formatDesc = fromTextureFormat(F);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    formatDesc.internal,
                    data->width, data->height,
                    0,
                    formatDesc.format,
                    formatDesc.type,
                    data->data
                );
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            tex.unBind();
            return tex;
        }

    private:
        explicit Texture(std::optional<TextureData> data = {}) {
            glGenTextures(1, &m_texture);
            if(!m_texture) {
                THROW_EXCEPTION("Could not create a texture.");
            }
        }

        GLuint m_texture = 0;
    };

    template<TextureFormat F>
    using Texture2d = Texture<2, unsigned char, F>;
    using Texture2dUint32 = Texture2d<TextureFormat::R32UInt>;
    using Texture2dUint16 = Texture2d<TextureFormat::R16Uint>;
}