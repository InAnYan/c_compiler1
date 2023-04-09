#include "Parser.hpp"

#include "Reporter.hpp"

namespace CComp
{
    Parser::Parser(Scanner& scanner)
        : m_Scanner(scanner)
    {}

    std::vector<std::unique_ptr<AST::Decl>> Parser::Parse()
    {
        Advance();

        std::vector<std::unique_ptr<AST::Decl>> res;

        while (!IsAtEnd())
        {
            res.push_back(ParseDecl());
        }

        if (!m_HadError)
        {
            return res;
        }
        else
        {
            return {};
        }
    }

    std::unique_ptr<AST::Decl> Parser::ParseDecl()
    {
        std::unique_ptr<AST::Type> returnType = ParseType();
        Token name = Require(TokenType::IDENTIFIER, "expected function name");
        Require(TokenType::LEFT_PAREN, "expected '(' after function name");
        Require(TokenType::RIGHT_PAREN, "expected ')' after function parameters");
        std::unique_ptr<AST::BlockStmt> body = ParseBlock();
    }

    bool Parser::IsAtEnd() const
    {
        return m_Next.type != TokenType::END_OF_FILE;
    }

    void Parser::Advance()
    {
        m_Current = m_Next;

        while (true)
        {
            m_Next = m_Scanner.NextToken();

            if (m_Next.type == TokenType::ERROR)
            {
                m_HadError = true;
                m_PanicMode = true;
                ReportError(m_Next.pos, m_Next.str);
                continue;
            }

            break;
        }
    }
} // namespace CComp
