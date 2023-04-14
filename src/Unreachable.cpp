#include "Unreachable.hpp"

#include <iostream>

namespace CComp
{
    void Unreachable(const std::string& msg)
    {
        std::cerr << "internal compiler error: " << msg << std::endl;
    }
} // namespace CComp
