#pragma once

#include "Exception.hpp"

/**
 * @brief Throws a linking exception.
 * 
 * @param compError The linking error. ( automatically moved to an std::string )
 */
#define THROW_LINK_EXCEPTION(linkError) throw renderer::LinkingException(__LINE__, __FILE__, std::string(linkError))

namespace renderer {
    /**
     * @brief Custom exception for handling shader linking errors.
     */
    class LinkingException : public voxels::Exception {
    public:
        explicit LinkingException(
            uint32 line,
            std::string&& file,
            std::string&& linkError
        ) noexcept
            : m_linkError(std::move(linkError))
            , voxels::Exception(line, std::move(file), "") {  }

    protected:
        virtual std::string formatMessage() const noexcept override {
            std::stringstream sstream;
            sstream << m_file << ":" << m_line << " Linking error: " << m_linkError;
            return sstream.str();
        }

    private:
        std::string m_linkError; //< The linking error.
    };
}