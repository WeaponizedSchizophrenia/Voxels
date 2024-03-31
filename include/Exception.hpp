#pragma once

#include "Global.hpp"
#include <exception> // For std::exception
#include <optional> // For caching a formated message
#include <sstream> // For formatting messages
#include <string> // For std::string

/// @brief Throws an exception with the provided message.
#define THROW_EXCEPTION(msg) throw voxels::Exception(__LINE__, __FILE__, msg)

namespace voxels {
    /**
     * @brief Custom exception type for easier debugging.
     */
    class Exception : public std::exception {
    public:
        [[nodiscard]] explicit Exception(uint32 line, std::string&& file, std::string&& message) noexcept
            : m_line(line), m_file(std::move(file)), m_message(std::move(message)) {  }

        [[nodiscard]] virtual const char* what() const noexcept override final {
            // If the message has not been formated yet, format it.
            if(!m_formatedMessage) {
                m_formatedMessage = formatMessage();
            }
            return m_formatedMessage->c_str();
        }

    protected:
        /**
         * @brief Formats the exception message into a string.
         * 
         * @return std::string The formated message.
         *
         * @note Derrived classes should override this function to format their custom message.
         */
        [[nodiscard]] inline virtual std::string formatMessage() const noexcept {
            std::stringstream sstream;
            sstream << m_file << ":" << m_line << " Exception: " << m_message;
            return sstream.str();
        }

    private:
        uint32 m_line; //< The line where the exception was thrown.
        std::string m_file; //< The file where the exception was thrown.
        std::string m_message; //< The message associated with the exception.
        mutable std::optional<std::string> m_formatedMessage; //< The formated message.
        // The formated message is mutable so that it could be accessed from the const char* what() function which is marked const.
    };
}