#include "Reporter.hpp"

#include <iostream>

namespace CComp
{
    void ReportError(FilePosition pos, const std::string& msg)
    {
        std::cerr << pos.line << ": error: " << msg << "." << std::endl;
    }
} // namespace CComp
