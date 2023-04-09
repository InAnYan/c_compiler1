#include "ArgParser.hpp"

namespace CComp
{
    
    ArgParser::ArgParser(int argc, char* argv[])
        : m_Args(argv, argv + argc)
    {}

    ScannerConfiguration ArgParser::GenerateScannerConfiguration() const
    {
        return ScannerConfiguration();
    }

    std::filesystem::path ArgParser::GetSourceFilePath() const
    {
        if (m_Args.size() > 0)
        {
            return m_Args[0];
        }
        else
        {
            return "";
        }
    }
}
