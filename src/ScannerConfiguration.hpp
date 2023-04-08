#ifndef CCOMP_SCANNER_CONFIGURATION_HPP
#define CCOMP_SCANNER_CONFIGURATION_HPP

#include <filesystem>

namespace CComp
{
    struct ScannerConfiguration
    {
        std::filesystem::path sourcePath;
        std::filesystem::path libraryPath;
        
        ScannerConfiguration()
            : sourcePath(std::filesystem::current_path()),
              libraryPath(std::filesystem::current_path())
        {}

        ScannerConfiguration(std::filesystem::path sourcePath, std::filesystem::path libraryPath)
            : sourcePath(sourcePath),
              libraryPath(libraryPath)
        {}
    }; // struct ScannerConfiguration

} // namespace CComp

#endif // CCOMP_SCANNER_CONFIGURATION_HPP
