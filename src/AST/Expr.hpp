#ifndef CCOMP_AST_EXPR_HPP
#define CCOMP_AST_EXPR_HPP

#include "Node.hpp"
#include "Visitor.hpp"

namespace CComp
{
    namespace AST
    {
        class Expr : public Node
        {
        public:
            Expr(FilePosition pos)
                : Node(pos)
            {}
        }; // class Expr

        class IntegerNumberExpr : public Expr
        {
        public:
            IntegerNumberExpr(FilePosition pos, const std::string& number)
                : Expr(pos),
                  number(number)
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitIntegerNumberExpr(*this);
            }

            std::string number;
        }; // class IntegerNumber

        enum class UnaryOp
        {
            NEGATION,
            BITWISE_COMPLEMENT,
            NOT,

            POST_INCREMENT,
            POST_DECREMENT,
            PRE_INCREMENT,
            PRE_DECREMENT,
        };

        class UnaryOpExpr : public Expr
        {
        public:
            UnaryOpExpr(FilePosition pos, UnaryOp op, std::unique_ptr<Expr> expr)
                : Expr(pos), op(op), expr(std::move(expr))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitUnaryOpExpr(*this);
            }

            UnaryOp op;
            std::unique_ptr<Expr> expr;
        }; // class UnaryOpExpr

        enum class BinaryOp
        {
            COMMA,
            
            ADD,
            SUBSTRACT,
            MULTIPLY,
            DIVIDE,
            MODULO,

            LOGIC_AND,
            BITWISE_AND,
            LOGIC_OR,
            BITWISE_OR,
            BITWISE_XOR,

            EQUAL,
            NOT_EQUAL,
            GREATER,
            GREATER_EQUAL,
            LESS,
            LESS_EQUAL,

            BITWISE_SHIFT_LEFT,
            BITWISE_SHIFT_RIGHT,
        }; // enum class BinaryOp

        class BinaryOpExpr : public Expr
        {
        public:
            BinaryOpExpr(FilePosition pos, BinaryOp op, std::unique_ptr<AST::Expr> left, std::unique_ptr<AST::Expr> right)
                : Expr(pos), op(op), left(std::move(left)), right(std::move(right))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitBinaryOpExpr(*this);
            }

            BinaryOp op;
            std::unique_ptr<AST::Expr> left;
            std::unique_ptr<AST::Expr> right;
        }; // class BinaryOpExpr

        enum class AssignOp
        {
            SIMPLE_ASSIGN,
            
            ADD_ASSIGN,
            SUBSTRACT_ASSIGN,
            MULTIPLY_ASSIGN,
            DIVIDE_ASSIGN,
            MODULO_ASSIGN,

            BITWISE_AND_ASSIGN,
            BITWISE_OR_ASSIGN,
            BITWISE_XOR_ASSIGN,

            BITWISE_SHIFT_LEFT_ASSIGN,
            BITWISE_SHIFT_RIGHT_ASSIGN,
        }; // enum class AssignOp

        class AssignExpr : public Expr
        {
        public:
            AssignExpr(FilePosition pos, AssignOp op, std::unique_ptr<AST::Expr> target, std::unique_ptr<AST::Expr> expr)
                : Expr(pos), op(op), target(std::move(target)), expr(std::move(expr))
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitAssignExpr(*this);
            }

            AssignOp op;
            std::unique_ptr<AST::Expr> target;
            std::unique_ptr<AST::Expr> expr;
        }; // class AssignExpr

        class VarExpr : public Expr
        {
        public:
            VarExpr(FilePosition pos, const std::string& name)
                : Expr(pos), name(name)
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitVarExpr(*this);
            }

            std::string name;
        }; // class VarExpr

    } // namespace AST
} // namespace CComp

#endif // CCOMP_AST_EXPR_HPP
