#include "Scanner.hpp"

#include "File/Reader.hpp"

namespace CComp
{
    std::unordered_map<std::string, TokenType> g_StringToKeyword =
    {
        { "int", TokenType::INT },
        { "return", TokenType::RETURN }
    };

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
        : m_Configuration(configuration)
    {
        BeginNewFile(file);
    }

    Token Scanner::NextToken()
    {
        SkipWhitespace();
        BeginNewToken();

        if (IsAtEnd())
        {
            if (!m_States.empty())
            {
                PopState();
                return NextToken();
            }

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

        case '-': return Match('-')
                ? MakeToken(TokenType::MINUS_MINUS)
                : Match('=')
                  ? MakeToken(TokenType::MINUS_EQUAL)
                  : MakeToken(TokenType::MINUS);

        case '+': return Match('+')
                ? MakeToken(TokenType::PLUS_PLUS)
                : Match('=')
                  ? MakeToken(TokenType::PLUS_EQUAL)
                  : MakeToken(TokenType::PLUS);

        case '*': return Match('=')
                ? MakeToken(TokenType::STAR_EQUAL)
                : MakeToken(TokenType::STAR);

        case '/': return Match('=')
                ? MakeToken(TokenType::SLASH_EQUAL)
                : MakeToken(TokenType::SLASH);

        case '%': return Match('=')
                ? MakeToken(TokenType::PERCENT_EQUAL)
                : MakeToken(TokenType::PERCENT);

        case '^': return Match('=')
                ? MakeToken(TokenType::UP_ARROW_EQUAL)
                : MakeToken(TokenType::UP_ARROW);

        case '|': return Match('=')
                ? MakeToken(TokenType::BAR_EQUAL)
                : Match('|')
                  ? MakeToken(TokenType::BAR_BAR)
                  : MakeToken(TokenType::BAR);

        case '&': return Match('=')
                ? MakeToken(TokenType::AMPERSAND_EQUAL)
                : Match('&')
                  ? MakeToken(TokenType::AMPERSAND_AMPERSAND)
                  : MakeToken(TokenType::AMPERSAND);

        case '~': return Match('=')
                ? MakeToken(TokenType::TILDA_EQUAL)
                : MakeToken(TokenType::TILDA);

        case '!': return Match('=')
                ? MakeToken(TokenType::BANG_EQUAL)
                : MakeToken(TokenType::BANG);

        case '=': return Match('=')
                ? MakeToken(TokenType::EQUAL_EQUAL)
                : MakeToken(TokenType::EQUAL);

        case '>': return Match('=')
                ? MakeToken(TokenType::GREATER_EQUAL)
                : Match('>')
                  ? (Match('=')
                    ? MakeToken(TokenType::GREATER_GREATER_EQUAL)
                    : MakeToken(TokenType::GREATER_GREATER))
                  : MakeToken(TokenType::GREATER);

        case '<': return Match('=')
                ? MakeToken(TokenType::LESS_EQUAL)
                : Match('<')
                  ? (Match('=')
                    ? MakeToken(TokenType::LESS_LESS_EQUAL)
                    : MakeToken(TokenType::LESS_LESS))
                  : MakeToken(TokenType::LESS);

        case '#': return Directive();

        default:
            if (IsAlpha(ch) || ch == '_')
            {
                return IdentifierOrKeyword();
            }
            else if (IsDigit(ch))
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

    void Scanner::BeginNewToken()
    {
        m_Start = m_Current;
    }

    void Scanner::BeginNewFile(std::shared_ptr<const File> file)
    {
        m_File = file;
        m_Line = 0;
        m_Start = file->begin();
        m_Current = m_Start;
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

    bool Scanner::Match(char ch)
    {
        if (IsAtEnd() || Peek() != ch)
        {
            return false;
        }

        Advance();
        return true;
    }

    Token Scanner::IdentifierOrKeyword()
    {
        AdvanceWhile([] (char ch)
        {
            return IsAlpha(ch) || IsDigit(ch) || ch == '_';
        });

        std::string name = std::string(m_Start, m_Current);

        if (m_Macro.count(name) != 0)
        {
            Macro& macro = m_Macro[name];

            PushState();
            BeginNewFile(std::make_shared<File>(macro.rewrite)); // That's a hack. What if File type changes?

            return NextToken();
        }

        return MakeToken(CheckKeyword(name));
    }

    Token Scanner::Number()
    {
        AdvanceWhile([] (char ch)
        {
            return IsDigit(ch);
        });

        return MakeToken(TokenType::INTEGER_NUMBER);
    }

    TokenType Scanner::CheckKeyword(const std::string& str) const
    {
        auto iter = g_StringToKeyword.find(str);

        if (iter != g_StringToKeyword.end())
        {
            return iter->second;
        }
        else
        {
            return TokenType::IDENTIFIER;
        }
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

    Token Scanner::Directive()
    {
        m_Start++;
        Token directive = IdentifierOrKeyword();

        SkipWhitespace();
        BeginNewToken();
        
        if (directive.str == "include")
        {
            return IncludeDirective();
        }
        else if (directive.str == "define")
        {
            return DefineDirective();
        }
        else
        {
            return MakeErrorToken("unknown preprocessor directive");
        }
    }

    Token Scanner::IncludeDirective()
    {
        if (IsAtEnd())
        {
            return MakeErrorToken("expected include path");
        }

        char endChar;
        switch (Advance())
        {
        case '"': endChar = '"'; break;
        case '<': endChar = '>'; break;
        default:
            return MakeErrorToken("expected '\"' or '<");
        }

        AdvanceWhile([endChar] (char ch)
        {
            return ch != endChar;
        });

        if (IsAtEnd())
        {
            return MakeErrorToken("expected end of include path");
        }

        Advance();

        std::string path(m_Start + 1, m_Current - 1);

        std::shared_ptr<const File> file = ReadFile(std::filesystem::path(path));
        if (file == nullptr)
        {
            return MakeErrorToken("unable to read file '" + path + "'");
        }

        PushState();
        BeginNewFile(file);

        return NextToken();
    }

    Token Scanner::DefineDirective()
    {
        if (IsAtEnd())
        {
            return MakeErrorToken("expected macro name");
        }

        SkipWhitespace();
        BeginNewToken();

        Token name = IdentifierOrKeyword();
        std::vector<char> rewrite;
        bool isFunction = false;
        size_t argCount = 0;
        
        SkipWhitespace();
        BeginNewToken();

        if (Peek() == '(')
        {
            AdvanceWhile([] (char ch)
            {
                return ch != ')';
            });

            if (IsAtEnd())
            {
                return MakeErrorToken("expected ')' after parameter list");
            }

            Advance();

            isFunction = true;
        }

        while (!IsAtEnd())
        {
            SkipWhitespace();
            BeginNewToken();

            AdvanceWhile([] (char ch)
            {
                return ch != '\n';
            });

            rewrite.insert(rewrite.end(), m_Start, m_Current);

            if (rewrite.back() == '\\')
            {
                rewrite.pop_back();
                rewrite.push_back('\n');
                continue;
            }

            break;
        }

        if (rewrite.back() == '\n')
        {
            rewrite.pop_back();
        }

        if (isFunction)
        {
            return MakeErrorToken("function-like macro are not supported");
        }
        else
        {
            m_Macro[name.str] = { isFunction, argCount, rewrite };

            return NextToken();
        }
    }

    void Scanner::PushState()
    {
        m_States.push_back({ m_File, m_Line, m_Start, m_Current });
    }

    void Scanner::PopState()
    {
        // assert !state.empty();
        
        ScannerState state = m_States.back();
        m_States.pop_back();

        m_File = state.file;
        m_Line = state.line;
        m_Start = state.start;
        m_Current = state.current;
    }
}
