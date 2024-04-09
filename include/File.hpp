#pragma once

#include <fstream> // For file stream.
#include <future> // For asynchronous IO.
#include <string_view> // For string view.

/*
    Current File is not very useful.
    However in the future as more features are needed it should be a lot
    more useful and easier to use then std::fstream.
*/

namespace voxels {
    /**
     * @brief std::fstream wrapper.
     */
    class File {
    public:
        explicit File(std::fstream&& file) noexcept : m_file(std::move(file)) {  }
        virtual ~File() noexcept {
            if(m_file.is_open()) {
                m_file.close();
            }
        }

    protected:
        std::fstream m_file; //< The file stream.
    };

    /**
     * @brief Extends the File class to provide reading functionality.
     */
    class FileReader: public File {
    public:
        using File::File;
        /**
         * @brief Asyncronously reads the content of the file.
         * 
         * @return std::future<std::string> A future containing the file contents as a string.
         */
        [[nodiscard]] std::future<std::string> readContent();
    };

    /**
     * @brief Extends the File class to provide writing functionality.
     */
    class FileWriter: public File {
    public:
        using File::File;
        /**
         * @brief Writes the content to the file.
         * 
         * @param content The content to write.
         */
        void writeContent(std::string_view content);
    };

    /**
     * @brief Opens a file for reading.
     * 
     * @param path The path to the file.
     * @return FileReader The file reader for the file.
     *
     * @throws voxels::Exception If the file does not exist.
     */
    [[nodiscard]] FileReader openForReading(std::string_view path);
    /**
     * @brief Opens a file for writing.
     * 
     * @param path The path to the file.
     * @param append A boolean value indicating if the file should be appended to.
     * @return FileWriter The file writer for the file.
     *
     * @throws voxels::Exception If the file already exists.
     */
    [[nodiscard]] FileWriter openForWriting(std::string_view path, bool append = false);
    /**
     * @brief Creates a file and opens it for writing.
     * 
     * @param path The path to the new file.
     * @return FileWriter The file writer for the new file.
     *
     * @throws voxels::Exception If the file already exists.
     */
    [[nodiscard]] FileWriter createAndWrite(std::string_view path);
}