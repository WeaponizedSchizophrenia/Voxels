#pragma once

#include "Exception.hpp"
#include "Renderer/AttributeDescriptor.hpp"
#include "Renderer/TextureFilter.hpp"
#include "Renderer/TextureFormat.hpp"
#include "Renderer/TextureWrap.hpp"
#include "glad/gl.h"

namespace renderer::opengl {
    /**
     * @brief Gets the size of the provided OpenGl type in bytes.
     * 
     * @param type The OpenGl type.
     * @return GLuint The size of the type in bytes.
     *
     * @throws voxels::Exception If the type is not valid.
     */
    [[nodiscard]] static constexpr inline GLuint getSizeOfType(GLenum type) {
        switch(type) {
            case GL_INT: return sizeof(GLint);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_FLOAT: return sizeof(GLfloat);
            default: THROW_EXCEPTION("Parameter [type] is not a valid OpenGL type");
        }
    }

    [[nodiscard]] static constexpr inline GLenum fromAttributeType(AttributeType type) {
        switch (type) {
            case AttributeType::Float32:            return GL_FLOAT;
            case AttributeType::Integer32:          return GL_INT;
            case AttributeType::Byte:               return GL_BYTE;
            case AttributeType::UnsignedInteger32:  return GL_UNSIGNED_INT;
            default: THROW_EXCEPTION("Parameter [type] is not a valid vertex attribute type");
        }
    }

    struct FormatDescriptor { GLenum internal, format, type; };
    [[nodiscard]] static constexpr inline FormatDescriptor fromTextureFormat(TextureFormat format) {
        switch(format) {
            case TextureFormat::R32UInt: return { GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT };
            case TextureFormat::R16Uint: return { GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT };
            default: THROW_EXCEPTION("Parameter [format] is not a valid texture format");
        }
    }

    [[nodiscard]] static constexpr inline GLenum fromTextureWrap(TextureWrap wrap) {
        switch(wrap) {
            case TextureWrap::Repeat: return GL_REPEAT;
            case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
            default: THROW_EXCEPTION("Parameter [wrap] is not a valid texture wrap");
        }
    }

    [[nodiscard]] static constexpr inline GLenum fromTextureFilter(TextureFilter filter) {
        switch(filter) {
            case TextureFilter::Nearest: return GL_NEAREST;
            case TextureFilter::Linear: return GL_LINEAR;
            default: THROW_EXCEPTION("Parameter [filter] is not a valid texture filter");
        }
    }
}