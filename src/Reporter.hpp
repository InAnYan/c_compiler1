#ifndef CCOMP_REPORTER_HPP
#define CCOMP_REPORTER_HPP

#include "File/FilePosition.hpp"

namespace CComp
{
    void ReportError(FilePosition pos, const std::string& msg);
} // namespace CComp

#endif // CCOMP_REPORTER_HPP
