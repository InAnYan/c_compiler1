#ifndef CCOMP_FILE_POSITION_HPP
#define CCOMP_FILE_POSITION_HPP

#include <memory>
#include <fstream>

#include "File.hpp"

namespace CComp
{
    struct FilePosition
    {
        size_t line;
        std::shared_ptr<const File> file;

        FilePosition(size_t line, std::shared_ptr<const File> file)
            : line(line),
              file(file)
        {}
    }; // struct FilePosition

} // namespace CComp

#endif // CCOMP_FILE_POSITION_HPP
