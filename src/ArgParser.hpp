#ifndef CCOMP_ARG_PARSER_HPP
#define CCOMP_ARG_PARSER_HPP

#include <vector>
#include <string>
#include <filesystem>

#include "ScannerConfiguration.hpp"

namespace CComp
{
    class ArgParser
    {
    public:
        ArgParser(int argc, char* argv[]);

        ScannerConfiguration GenerateScannerConfiguration() const;

        std::filesystem::path GetSourceFilePath() const;
        
    private:
        std::vector<std::string> m_Args;
    }; // class ArgParser

} // namespace CComp

#endif // CCOMP_ARG_PARSER_HPP
