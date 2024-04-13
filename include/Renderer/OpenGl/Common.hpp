#pragma once

#include "Exception.hpp"
#include "Renderer/AttributeDescriptor.hpp"
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
}