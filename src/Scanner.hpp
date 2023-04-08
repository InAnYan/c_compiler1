#ifndef CCOMP_SCANNER_HPP
#define CCOMP_SCANNER_HPP

#include <memory>
#include <fstream>
#include <functional>

#include "File.hpp"
#include "TokenType.hpp"
#include "Token.hpp"
#include "ScannerConfiguration.hpp"

namespace CComp
{
    bool IsAlpha(char ch);
    bool IsDigit(char ch);

    class Scanner
    {
    public:
        Scanner(const ScannerConfiguration& configuration, std::shared_ptr<const File> file);

        Token NextToken();
        
    private:
        ScannerConfiguration m_Configuration;
        std::shared_ptr<const File> m_File;

        size_t m_Line;
        File::const_iterator m_Start;
        File::const_iterator m_Current;

        void SkipWhitespace();

        bool IsAtEnd() const;
        char Peek(size_t offset = 0) const;

        char Advance();
        void AdvanceWhile(const std::function<bool(char)> func);

        Token IdentifierOrKeyword();
        Token Number();

        TokenType CheckKeyword() const;
        TokenType CheckRest(TokenType keyword, const std::string& str, size_t count) const;

        Token MakeToken(TokenType type) const;
        Token MakeErrorToken(const std::string& msg) const;

        FilePosition MakeCurrentPosition() const;
    }; // class Scanner

} // namespace CComp

#endif // CCOMP_SCANNER_HPP
