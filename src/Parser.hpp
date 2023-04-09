#ifndef CCOMP_PARSER_HPP
#define CCOMP_PARSER_HPP

#include <memory>
#include <vector>

#include "Scanner.hpp"
#include "AST/AST.hpp"

namespace CComp
{
    class Parser
    {
    public:
        Parser(Scanner& scanner);

        std::vector<std::unique_ptr<AST::Decl>> Parse();
    private:
        Scanner& m_Scanner; 

        bool m_HadError = false;
        bool m_PanicMode = false;

        Token m_Current;
        Token m_Next;

        bool IsAtEnd() const;

        void Advance();

        Token Require(TokenType type, const std::string& errorMsg);

        std::unique_ptr<AST::Decl> ParseDecl();
        std::unique_ptr<AST::Type> ParseType();

        std::unique_ptr<AST::BlockStmt> ParseBlock();
    }; // class Parser

} // namespace CComp

#endif // CCOMP_PARSER_HPP
