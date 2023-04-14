#include "DebugPrintVisitor.hpp"

#include "../AST/AST.hpp"

namespace CComp
{
    DebugPrintVisitor::DebugPrintVisitor(std::ostream& out)
        : out(out)
    {}

    void DebugPrintVisitor::VisitBasicType(AST::BasicType& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "BasicType: " << node.name << std::endl;
    }

    void DebugPrintVisitor::VisitTypeAndNameDecl(AST::TypeAndNameDecl& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "TypeAndNameDecl: " << node.name << std::endl;
    }

    void DebugPrintVisitor::VisitFunctionDecl(AST::FunctionDecl& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "FunctionDecl: " << node.name << std::endl;

        tab++;

        node.returnType->Accept(*this);

        for (auto& param : node.parameters)
        {
            param->Accept(*this);
        }

        node.body->Accept(*this);

        tab--;
    }

    void DebugPrintVisitor::VisitIntegerNumberExpr(AST::IntegerNumberExpr& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "IntegerNumberExpr: " << node.number << std::endl;
    }

    void DebugPrintVisitor::VisitBlockStmt(AST::BlockStmt& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "BlockStmt: " << std::endl;

        tab++;

        for (auto& stmt : node.stmts)
        {
            stmt->Accept(*this);
        }

        tab--;
    }

    void DebugPrintVisitor::VisitReturnStmt(AST::ReturnStmt& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "ReturnStmt: " << std::endl;

        tab++;

        node.expr->Accept(*this);

        tab--;
    }

    void DebugPrintVisitor::VisitUnaryOpExpr(AST::UnaryOpExpr& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "UnaryOpExpr: " << static_cast<int>(node.op) << std::endl;

        tab++;

        node.expr->Accept(*this);

        tab--;
    }

    void DebugPrintVisitor::VisitBinaryOpExpr(AST::BinaryOpExpr& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "BinaryOpExpr: " << static_cast<int>(node.op) << std::endl;

        tab++;

        node.left->Accept(*this);
        node.right->Accept(*this);

        tab--;
    }

    void DebugPrintVisitor::VisitVarDeclStmt(AST::VarDeclStmt& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "VarDeclStmt: " << std::endl;

        if (node.init != nullptr)
        {
            tab++;
            node.init->Accept(*this);
            tab--;
        }
    }

    void DebugPrintVisitor::VisitExprStmt(AST::ExprStmt& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "ExprStmt: " << std::endl;

        tab++;
        node.expr->Accept(*this);
        tab--;
    }

    void DebugPrintVisitor::VisitVarExpr(AST::VarExpr& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "VarStmt: " << node.name << std::endl;
    }

    void DebugPrintVisitor::VisitAssignExpr(AST::AssignExpr& node)
    {
        out << std::string(tab, ' ');
        out << node.pos.line << ":" << "AssignExpr: " << static_cast<int>(node.op) << std::endl;

        tab++;

        node.target->Accept(*this);
        node.expr->Accept(*this);

        tab--;
    }
} // namespace CComp
