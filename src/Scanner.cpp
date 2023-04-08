#include "Scanner.hpp"

namespace CComp
{
    bool IsAlpha(char ch)
    {
        return (ch >= 'a' && ch <= 'z')
            || (ch >= 'A' && ch <= 'Z');
    }

    bool IsDigit(char ch)
    {
        return ch >= '0' && ch <= '9';
    }

    Scanner::Scanner(const ScannerConfiguration& configuration, std::shared_ptr<const File> file)
        : m_Configuration(configuration), m_File(file),
          m_Line(0),
          m_Start(file->begin()),
          m_Current(m_Start)
    {}

    Token Scanner::NextToken()
    {
        SkipWhitespace();

        m_Start = m_Current;

        if (IsAtEnd())
        {
            return MakeToken(TokenType::END_OF_FILE);
        }

        char ch = Advance();

        switch (ch)
        {
        case ';': return MakeToken(TokenType::SEMICOLON);
        case '(': return MakeToken(TokenType::LEFT_PAREN);
        case ')': return MakeToken(TokenType::RIGHT_PAREN);
        case '{': return MakeToken(TokenType::LEFT_BRACKET);
        case '}': return MakeToken(TokenType::RIGHT_BRACKET);

        default:
            if (IsAlpha(Peek()) || Peek() == '_')
            {
                return IdentifierOrKeyword();
            }
            else if (IsDigit(Peek()))
            {
                return Number();
            }
            else
            {
                return MakeErrorToken("unrecognized character");
            }
        }
    }

    void Scanner::SkipWhitespace()
    {
        // Possible use of AdvanceWhile.
        
        while (!IsAtEnd())
        {
            switch (Peek())
            {
            case ' ':
            case '\t':
                Advance();
                break;
            case '\n':
                m_Line++;
                Advance();
                break;
            default:
                return;
            }
        }
    }

    char Scanner::Peek(size_t offset) const
    {
        return m_Current[offset];
    }

    bool Scanner::IsAtEnd() const
    {
        return m_Current == m_File->end();
    }

    char Scanner::Advance()
    {
        m_Current++;
        return m_Current[-1];
    }

    void Scanner::AdvanceWhile(const std::function<bool(char)> func)
    {
        while (!IsAtEnd() && func(Peek()))
        {
            Advance();
        }
    }

    Token Scanner::IdentifierOrKeyword()
    {
        AdvanceWhile([] (char ch)
        {
            return IsAlpha(ch) || IsDigit(ch) || ch == '_';
        });

        return MakeToken(CheckKeyword());
    }

    Token Scanner::Number()
    {
        AdvanceWhile([] (char ch)
        {
            return IsDigit(ch);
        });

        return MakeToken(TokenType::INTEGER_NUMBER);
    }

    TokenType Scanner::CheckKeyword() const
    {
        // assert m_Current != m_Start;

        switch (*m_Start)
        {
        case 'i': return CheckRest(TokenType::INT, "nt", 1);
        case 'r': return CheckRest(TokenType::RETURN, "eturn", 1);
        default:  return TokenType::IDENTIFIER;
        }
    }

    TokenType Scanner::CheckRest(TokenType keyword, const std::string& str, size_t count) const
    {
        for (size_t i = count; i - count < str.size(); i++)
        {
            if (*(m_Start + i) != str[i - count])
            {
                return TokenType::IDENTIFIER;
            }
        }

        return keyword;
    }

    Token Scanner::MakeToken(TokenType type) const
    {
        return Token(type, std::string(m_Start, m_Current), MakeCurrentPosition());
    }

    Token Scanner::MakeErrorToken(const std::string& msg) const
    {
        return Token(TokenType::ERROR, msg, MakeCurrentPosition());
    }

    FilePosition Scanner::MakeCurrentPosition() const
    {
        return FilePosition(m_Line, m_File);
    }
}
