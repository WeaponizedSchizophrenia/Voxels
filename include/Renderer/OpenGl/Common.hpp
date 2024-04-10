#pragma once

#include "Exception.hpp"
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
    [[nodiscard]] inline GLuint getSizeOfType(GLenum type) {
        switch(type) {
            case GL_INT: return sizeof(GLint);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_FLOAT: return sizeof(GLfloat);
            default: THROW_EXCEPTION("Parameter [type] is not a valid OpenGL type");
        }
    }
}