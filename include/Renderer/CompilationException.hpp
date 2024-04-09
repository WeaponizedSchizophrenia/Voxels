#pragma once

#include "Exception.hpp"
#include "Renderer/ShaderType.hpp"

/**
 * @brief Throws a compilation exception.
 * 
 * @param type The shader type.
 * @param compError The compilation error. ( automatically moved to an std::string )
 */
#define THROW_COMP_EXCEPTION(type, compError) throw renderer::CompilationException(__LINE__, __FILE__, std::string(std::move(compError)), type)

namespace renderer {
    /// @brief The size of the info log buffer.
    static constexpr const size_t INFO_LOG_BUFFER_SIZE = 512;

    /**
     * @brief Custom exception for handling shader compilation errors.
     */
    class CompilationException : public voxels::Exception {
    public:
        explicit CompilationException(
            uint32 line,
            std::string&& file,
            std::string&& compError,
            renderer::ShaderType shaderType
        ) noexcept
            : m_compError(std::move(compError))
            , m_shaderType(shaderType)
            , voxels::Exception(line, std::move(file), "") {  }

    protected:
        virtual std::string formatMessage() const noexcept override {
            std::stringstream sstream;
            auto type = [this] {
                switch(m_shaderType) {
                    case renderer::ShaderType::Vertex: return "Vertex shader";
                    case renderer::ShaderType::Fragment: return "Fragment shader";
                    default: return "Shader";
                }
            }();
            sstream << m_file << ":" << m_line << ' ' << type << " compilation error: " << m_compError;
            return sstream.str();
        }

    private:
        renderer::ShaderType m_shaderType; //< The shader type.
        std::string m_compError; //< The compilation error.
    };
}