#ifndef CCOMP_TOKEN_HPP
#define CCOMP_TOKEN_HPP

#include <string>

#include "TokenType.hpp"
#include "../File/FilePosition.hpp"

namespace CComp
{
    struct Token
    {
        TokenType type;
        std::string str;
        FilePosition pos;

        Token(TokenType type, const std::string& str, FilePosition pos)
            : type(type),
              str(str),
              pos(pos)
        {}

        Token()
            : type(TokenType::IDENTIFIER), str("null"), pos(FilePosition(0, nullptr))
        {}
    }; // struct Token

} // namespace CComp

#endif // CCOMP_TOKEN_HPP
