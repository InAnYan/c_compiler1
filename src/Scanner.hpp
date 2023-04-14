#ifndef CCOMP_SCANNER_HPP
#define CCOMP_SCANNER_HPP

#include <memory>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <vector>

#include "File/File.hpp"
#include "Token/TokenType.hpp"
#include "Token/Token.hpp"
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

        struct ScannerState
        {
            std::shared_ptr<const File> file;
            size_t line;
            File::const_iterator start;
            File::const_iterator current;
        }; // struct ScannerState

        std::vector<ScannerState> m_States;

        void PushState();
        void PopState();

        struct Macro
        {
            bool isFunction;
            size_t argCount;
            std::vector<char> rewrite;
        }; // struct Macro

        std::unordered_map<std::string, Macro> m_Macro;

        void SkipWhitespace();

        void BeginNewToken();
        void BeginNewFile(std::shared_ptr<const File> file);

        bool IsAtEnd() const;
        char Peek(size_t offset = 0) const;

        char Advance();
        void AdvanceWhile(const std::function<bool(char)> func);
        bool Match(char ch);

        Token IdentifierOrKeyword();
        Token Number();

        TokenType CheckKeyword(const std::string& str) const;

        Token MakeToken(TokenType type) const;
        Token MakeErrorToken(const std::string& msg) const;

        FilePosition MakeCurrentPosition() const;

        Token Directive();
        Token IncludeDirective();
        Token DefineDirective();
    }; // class Scanner

} // namespace CComp

#endif // CCOMP_SCANNER_HPP
