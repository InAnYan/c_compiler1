#include <iostream>

#include "Scanner.hpp"

using namespace CComp;

int main()
{
    std::shared_ptr<File> file = std::make_shared<File>();

    std::string src = "int main() { \nreturn ;}";

    for (char ch : src)
    {
        file->push_back(ch);
    }

    Scanner scanner(ScannerConfiguration(), file);

    while (true)
    {
        Token token = scanner.NextToken();
        std::cout << static_cast<int>(token.type) << ":" << token.pos.line << ": " << token.str << std::endl;

        if (token.type == TokenType::END_OF_FILE)
        {
            break;
        }
    }

    return 0;
}
