#ifndef CCOMP_DEBUG_PRINT_VISITOR_HPP
#define CCOMP_DEBUG_PRINT_VISITOR_HPP

#include "../AST/Visitor.hpp"

#include <iostream>

namespace CComp
{
    class DebugPrintVisitor : public AST::Visitor
    {
    public:
        DebugPrintVisitor(std::ostream& out);

        virtual void VisitBasicType(AST::BasicType& node);
        virtual void VisitTypeAndNameDecl(AST::TypeAndNameDecl& node);
        virtual void VisitFunctionDecl(AST::FunctionDecl& node);
        virtual void VisitIntegerNumberExpr(AST::IntegerNumberExpr& node);
        virtual void VisitBlockStmt(AST::BlockStmt& node);
        virtual void VisitReturnStmt(AST::ReturnStmt& node);
        virtual void VisitUnaryOpExpr(AST::UnaryOpExpr& node);
        virtual void VisitBinaryOpExpr(AST::BinaryOpExpr& node);
        virtual void VisitVarDeclStmt(AST::VarDeclStmt& node);
        virtual void VisitExprStmt(AST::ExprStmt& node);
        virtual void VisitVarExpr(AST::VarExpr& node);
        virtual void VisitAssignExpr(AST::AssignExpr& node);

    private:
        std::ostream& out;
        size_t tab = 0;
    }; // class DebugPrintVisitor
} // namespace CComp

#endif // CCOMP_DEBUG_PRINT_VISITOR_HPP
