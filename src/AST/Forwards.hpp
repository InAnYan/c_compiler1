#ifndef CCOMP_AST_FORWARDS_HPP
#define CCOMP_AST_FORWARDS_HPP

namespace CComp
{
    namespace AST
    {
        // Node.hpp
        class Node;

        // Decl.hpp
        class Decl;
        class TypeAndNameDecl;
        // using Parameter = TypeAndName;
        class FunctionDecl;
        
        // Stmt.hpp
        class Stmt;
        class BlockStmt;
        class ReturnStmt;

        // Type.hpp
        class Type;
        class BasicType;

        // Expr.hpp
        class Expr;
        class IntegerNumberExpr;
    } // namespace AST
} // namespace CComp

#endif // CCOMP_AST_FORWARDS_HPP
