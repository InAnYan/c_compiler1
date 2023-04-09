#ifndef CCOMP_FILE_READER_HPP
#define CCOMP_FILE_READER_HPP

#include <memory>
#include <filesystem>

#include "File.hpp"

namespace CComp
{
    std::shared_ptr<File> ReadFile(std::filesystem::path path);
} // namespace CComp

#endif // CCOMP_FILE_READER_HPP
