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

        MINUS,
        MINUS_MINUS,
        MINUS_EQUAL,
        PLUS,
        PLUS_PLUS,
        PLUS_EQUAL,
        STAR,
        STAR_EQUAL,
        SLASH,
        SLASH_EQUAL,
        TILDA,
        TILDA_EQUAL,
        BANG,
        UP_ARROW,
        UP_ARROW_EQUAL,
        COMMA,
        PERCENT,
        PERCENT_EQUAL,

        LESS_LESS,
        LESS_LESS_EQUAL,
        GREATER_GREATER,
        GREATER_GREATER_EQUAL,

        AMPERSAND_AMPERSAND,
        AMPERSAND,
        AMPERSAND_EQUAL,
        BAR_BAR,
        BAR,
        BAR_EQUAL,
        EQUAL_EQUAL,
        EQUAL,
        BANG_EQUAL,

        LESS,
        LESS_EQUAL,
        GREATER,
        GREATER_EQUAL,

        END_OF_FILE,
        ERROR,
    }; // enum class TokenType

} // namespace CComp

#endif // CCOMP_TYPE_HPP
