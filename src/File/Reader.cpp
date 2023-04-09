#include "Reader.hpp"

#include <fstream>

namespace CComp
{
    std::shared_ptr<File> ReadFile(std::filesystem::path path)
    {
        std::ifstream file(path);
        if (!file)
        {
            return nullptr;
        }

        std::shared_ptr<File> res =
            std::make_shared<File>(std::istreambuf_iterator<char>(file),
                                   std::istreambuf_iterator<char>());

        return res;
    }
}
