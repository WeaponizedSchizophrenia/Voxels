#include "File.hpp" // For declarations.
#include "Exception.hpp" // For exceptions.
#include <filesystem> // For checking if a file exists.

std::future<std::string> voxels::FileReader::readContent() {
    return std::async(std::launch::async, [this] {
        std::stringstream bytes;
        bytes << m_file.rdbuf();
        return bytes.str();
    });
}

void voxels::FileWriter::writeContent(std::string_view content) {
    m_file << content;
}

std::fstream open(std::string_view path, std::ios::openmode mode) {
    auto file = std::fstream(path.data(), mode);
    file.exceptions(std::ios::failbit | std::ios::badbit);
    return file;
}


voxels::FileReader voxels::openForReading(std::string_view path) {
    if(!std::filesystem::exists(path.data())) {
        THROW_EXCEPTION("File does not exist.");
    }

    return voxels::FileReader(open(std::move(path), std::ios::in));
}

voxels::FileWriter voxels::openForWriting(std::string_view path, bool append) {
    if(!std::filesystem::exists(path.data())) {
        THROW_EXCEPTION("File does not exist.");
    }

    return voxels::FileWriter(append 
        ? open(std::move(path), std::ios::out | std::ios::app)
        : open(std::move(path), std::ios::out | std::ios::trunc));
}

voxels::FileWriter voxels::createAndWrite(std::string_view path) {
    if(std::filesystem::exists(path.data())) {
        THROW_EXCEPTION("File already exists.");
    }

    return voxels::FileWriter(open(std::move(path), std::ios::out | std::ios::trunc));
}