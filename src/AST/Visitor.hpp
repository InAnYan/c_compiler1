#ifndef CCOMP_AST_VISITOR_HPP
#define CCOMP_AST_VISITOR_HPP

#include "Forwards.hpp"

namespace CComp
{
    namespace AST
    {
        class Visitor
        {
        public:
            virtual ~Visitor() = default;

            virtual void VisitBasicType(BasicType& node) = 0;
            virtual void VisitTypeAndNameDecl(TypeAndNameDecl& node) = 0;
            virtual void VisitFunctionDecl(FunctionDecl& node) = 0;
            virtual void VisitIntegerNumberExpr(IntegerNumberExpr& node) = 0;
            virtual void VisitBlockStmt(BlockStmt& node) = 0;
            virtual void VisitReturnStmt(ReturnStmt& node) = 0;
            virtual void VisitUnaryOpExpr(UnaryOpExpr& node) = 0;
            virtual void VisitBinaryOpExpr(BinaryOpExpr& node) = 0;
            virtual void VisitVarDeclStmt(VarDeclStmt& node) = 0;
            virtual void VisitExprStmt(ExprStmt& node) = 0;
            virtual void VisitVarExpr(VarExpr& node) = 0;
            virtual void VisitAssignExpr(AssignExpr& node) = 0;
        }; // class Visitor
    } // namespace AST
    
} // namespace CComp

#endif // CCOMP_AST_VISITOR_HPP
