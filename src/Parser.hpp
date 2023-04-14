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
        bool Check(TokenType type) const;

        void Advance();

        void Consume(TokenType type, const std::string& errorMsg);

        bool Match(TokenType type);

        template <typename... Args>
        bool Match(TokenType type, Args... rest)
        {
            if (Match(type))
            {
                return true;
            }
            else
            {
                return Match(rest...);
            }
        }

        void ErrorAtCurrent(const std::string& errorMsg);
        void ErrorAtNext(const std::string& errorMsg);

        void Synchronize();

        AST::UnaryOp TokenTypeToUnaryOp(TokenType type, bool isPrefix) const;
        AST::BinaryOp TokenTypeToBinaryOp(TokenType type) const;
        AST::AssignOp TokenTypeToAssignOp(TokenType type) const;

        std::unique_ptr<AST::Decl> ParseDecl();
        std::unique_ptr<AST::Stmt> ParseStmt();
        std::unique_ptr<AST::Expr> ParseExpr();
        std::unique_ptr<AST::Type> ParseType();

        std::unique_ptr<AST::Expr> ParseCommaExpr();
        std::unique_ptr<AST::Expr> ParseAssignmentExpr();
        std::unique_ptr<AST::Expr> ParseLogicalOrExpr();
        std::unique_ptr<AST::Expr> ParseLogicalAndExpr();
        std::unique_ptr<AST::Expr> ParseBitwiseOrExpr();
        std::unique_ptr<AST::Expr> ParseBitwiseXorExpr();
        std::unique_ptr<AST::Expr> ParseBitwiseAndExpr();
        std::unique_ptr<AST::Expr> ParseBitwiseShiftExpr();
        std::unique_ptr<AST::Expr> ParseEqualityExpr();
        std::unique_ptr<AST::Expr> ParseComparisonExpr();
        std::unique_ptr<AST::Expr> ParseTermExpr();
        std::unique_ptr<AST::Expr> ParseFactorExpr();
        std::unique_ptr<AST::Expr> ParseUnaryExpr();
        std::unique_ptr<AST::Expr> ParsePrimaryExpr();

        std::unique_ptr<AST::BlockStmt> ParseBlock();
        std::unique_ptr<AST::ReturnStmt> ParseReturnStmt();
        std::unique_ptr<AST::ExprStmt> ParseExprStmt();
        std::unique_ptr<AST::VarDeclStmt> ParseVarDeclStmt();
    }; // class Parser

} // namespace CComp

#endif // CCOMP_PARSER_HPP
