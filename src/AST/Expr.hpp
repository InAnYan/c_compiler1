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
    } // namespace AST
} // namespace CComp

#endif // CCOMP_AST_EXPR_HPP
