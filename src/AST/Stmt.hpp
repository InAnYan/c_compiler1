#ifndef CCOMP_AST_STMT_HPP
#define CCOMP_AST_STMT_HPP

#include "Node.hpp"
#include "Forwards.hpp"
#include "Visitor.hpp"

namespace CComp
{
    namespace AST
    {
        class Stmt : public Node
        {
        public:
            Stmt(FilePosition pos)
                : Node(pos)
            {}
        };

        class BlockStmt : public Stmt
        {
        public:
            BlockStmt(FilePosition pos, std::vector<std::unique_ptr<Stmt>> stmts)
                : Stmt(pos),
                  stmts(std::move(stmts))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitBlockStmt(*this);
            }

            std::vector<std::unique_ptr<Stmt>> stmts;
        }; // class Block

        class ReturnStmt : public Stmt
        {
        public:
            ReturnStmt(FilePosition pos, std::unique_ptr<Expr> expr)
                : Stmt(pos),
                  expr(std::move(expr))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitReturnStmt(*this);
            }

            std::unique_ptr<Expr> expr;
        }; // class ReturnStmt
        
        class ExprStmt : public Stmt
        {
        public:
            ExprStmt(FilePosition pos, std::unique_ptr<Expr> expr)
                : Stmt(pos),
                  expr(std::move(expr))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitExprStmt(*this);
            }

            std::unique_ptr<Expr> expr;
        }; // class ExprStmt
        
        class VarDeclStmt : public Stmt
        {
        public:
            VarDeclStmt(FilePosition pos, std::unique_ptr<Type> type, const std::string& name, std::unique_ptr<Expr> init)
                : Stmt(pos), type(std::move(type)), name(name), init(std::move(init))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitVarDeclStmt(*this);
            }

            std::unique_ptr<Type> type;
            std::string name;
            std::unique_ptr<Expr> init;
        }; // class VarDecl

    } // namespace AST
} // namespace CComp

#endif // CCOMP_AST_STMT_HPP
