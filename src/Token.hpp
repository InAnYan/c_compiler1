#ifndef CCOMP_TOKEN_HPP
#define CCOMP_TOKEN_HPP

#include <string>

#include "TokenType.hpp"
#include "FilePosition.hpp"

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
    }; // struct Token

} // namespace CComp

#endif // CCOMP_TOKEN_HPP
