#ifndef CCOMP_CODE_GEN_VISITOR_HPP
#define CCOMP_CODE_GEN_VISITOR_HPP

#include "../AST/Visitor.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

namespace CComp
{
    class CodeGenVisitor : public AST::Visitor
    {
    public:
        CodeGenVisitor(std::ostream& out);

        void Compile(std::vector<std::unique_ptr<AST::Decl>>& ast);

        bool HadError() const
        { return hadError; }

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
        bool hadError = false;
        std::ostream& out;
        size_t tab = 0;

        const int WORD_SIZE = 4;
        int currentStackIndex = -WORD_SIZE;
        std::unordered_map<std::string, int> vars;

        int MaybeGetVarIndex(std::unique_ptr<AST::Expr>& node);
        
        void EmitProlog();
        void EmitEpilog();

        std::ostream& Print();

        void MakeHeader();
        void MakeFooter();

        void BinaryComparison(const std::string& setOp);

        void SimpleBinary(AST::BinaryOpExpr& node);
        void SimpleBinaryOnEaxEbx(AST::BinaryOp op);
        void ShortCircuitBinary(AST::BinaryOpExpr& node);

        size_t labelCount = 0;
        std::string GenerateUniqueLabel();

        AST::BinaryOp AssignOpToBinaryOp(AST::AssignOp op) const;
    }; // class CodeGenVisitor
} // namespace CComp

#endif // CCOMP_CODE_GEN_VISITOR_HPP
