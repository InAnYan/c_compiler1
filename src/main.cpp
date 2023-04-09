#include <iostream>

#include "ArgParser.hpp"
#include "File/Reader.hpp"

#include "Scanner.hpp"
#include "Parser.hpp"

int main(int argc, char* argv[])
{
    CComp::ArgParser argParser(argc - 1, argv + 1);

    std::filesystem::path sourceFilePath = argParser.GetSourceFilePath();
    if (sourceFilePath.empty())
    {
        std::cerr << "Error: no source file provided." << std::endl;
        return 1;
    }

    std::shared_ptr<CComp::File> file = CComp::ReadFile(sourceFilePath);
    if (file == nullptr)
    {
        std::cerr << "Error: could not read the source file." << std::endl;
        return 1;
    }

    CComp::Scanner scanner(argParser.GenerateScannerConfiguration(), file);

    while (true)
    {
        CComp::Token token = scanner.NextToken();
        std::cout << static_cast<int>(token.type) << ":" << token.pos.line << ": " << token.str << std::endl;

        if (token.type == CComp::TokenType::END_OF_FILE)
        {
            break;
        }
    }

    return 0;
}
