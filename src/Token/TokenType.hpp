#ifndef CCOMP_TOKEN_TYPE_HPP
#define CCOMP_TOKEN_TYPE_HPP

namespace CComp
{
    enum class TokenType
    {
        INT,
        RETURN,

        IDENTIFIER,
        INTEGER_NUMBER,

        SEMICOLON,
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACKET,
        RIGHT_BRACKET,

        END_OF_FILE,
        ERROR,
    }; // enum class TokenType

} // namespace CComp

#endif // CCOMP_TYPE_HPP
