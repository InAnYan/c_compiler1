#include "CodeGenVisitor.hpp"

#include "../AST/AST.hpp"
#include "../Reporter.hpp"
#include "../Unreachable.hpp"

namespace CComp
{
    constexpr size_t gc_TabWidth = 4;

    CodeGenVisitor::CodeGenVisitor(std::ostream& out)
        : out(out)
    {}

    std::ostream& CodeGenVisitor::Print()
    {
        return out << std::string(tab * gc_TabWidth, ' ');
    }

    void CodeGenVisitor::Compile(std::vector<std::unique_ptr<AST::Decl>>& ast)
    {
        MakeHeader();

        for (auto& decl : ast)
        {
            decl->Accept(*this);
        }

        MakeFooter();
    }        
    
    void CodeGenVisitor::MakeHeader()
    {
        Print() << "# Compiled using InAnYan first C compiler v0.1." << std::endl;
        Print() << std::endl;
        Print() << ".globl _start" << std::endl;
        Print() << "_start:" << std::endl;
        tab++;
        Print() << "call main" << std::endl;
        Print() << "movl %eax, %ebx" << std::endl;
        Print() << "movl $1, %eax" << std::endl;
        Print() << "int $0x80" << std::endl;
        tab--;
        Print() << std::endl;
    }

    void CodeGenVisitor::MakeFooter()
    {
        Print() << "# End of compiling." << std::endl;
    }

    void CodeGenVisitor::VisitBasicType(AST::BasicType& node)
    {}

    void CodeGenVisitor::VisitTypeAndNameDecl(AST::TypeAndNameDecl& node)
    {}

    void CodeGenVisitor::VisitFunctionDecl(AST::FunctionDecl& node)
    {
        // TODO: Add prototype and source position.
        Print() << ".globl " << node.name << std::endl;
        Print() << node.name << ":" << std::endl;
        tab++;

        EmitProlog();

        node.body->Accept(*this);

        Print() << "ret" << std::endl; // TODO: If last statement was a return statement.
        tab--;
        Print() << std::endl;
    }

    void CodeGenVisitor::VisitIntegerNumberExpr(AST::IntegerNumberExpr& node)
    {
        Print() << "movl $" << node.number << ", %eax" << std::endl;
    }

    void CodeGenVisitor::VisitBlockStmt(AST::BlockStmt& node)
    {
        for (auto& stmt : node.stmts)
        {
            stmt->Accept(*this);
        }

        Print() << std::endl;
    }

    void CodeGenVisitor::VisitReturnStmt(AST::ReturnStmt& node)
    {
        node.expr->Accept(*this);
        EmitEpilog();
        Print() << "ret" << std::endl;
    }

    void CodeGenVisitor::VisitUnaryOpExpr(AST::UnaryOpExpr& node)
    {
        node.expr->Accept(*this);

        switch (node.op)
        {
        case AST::UnaryOp::NEGATION:
            Print() << "neg %eax" << std::endl;
            return;
        case AST::UnaryOp::NOT:
            Print() << "cmpl $0, %eax" << std::endl;
            Print() << "xor %eax, %eax" << std::endl;
            Print() << "sete %al" << std::endl;
            return;
        case AST::UnaryOp::BITWISE_COMPLEMENT:
            Print() << "not %eax" << std::endl;
            return;

        default:
            break;
        }

        int stackIndex = MaybeGetVarIndex(node.expr);
        Print() << "movl " << stackIndex << "(%ebp), %eax" << std::endl;

        switch (node.op)
        {
        case AST::UnaryOp::PRE_INCREMENT:
            Print() << "incl %eax" << std::endl;
            Print() << "movl %eax, " << stackIndex << "(%ebp)" << std::endl;
            break;
        case AST::UnaryOp::PRE_DECREMENT:
            Print() << "decl %eax" << std::endl;
            Print() << "movl %eax, " << stackIndex << "(%ebp)" << std::endl;
            break;

        case AST::UnaryOp::POST_INCREMENT:
            Print() << "addl $1, " << stackIndex << "(%ebp)" << std::endl;
            break;
        case AST::UnaryOp::POST_DECREMENT:
            Print() << "subl $1, " << stackIndex << "(%ebp)" << std::endl;
            break;

        default:
            break;
        }
    }

    void CodeGenVisitor::VisitBinaryOpExpr(AST::BinaryOpExpr& node)
    {
        if (node.op == AST::BinaryOp::COMMA)
        {
            node.left->Accept(*this);
            node.right->Accept(*this);
        }
        else if (node.op == AST::BinaryOp::LOGIC_OR
            || node.op == AST::BinaryOp::LOGIC_AND)
        {
            ShortCircuitBinary(node);
        }
        else
        {
            SimpleBinary(node);
        }

        Print() << std::endl;
    }
    
    void CodeGenVisitor::BinaryComparison(const std::string& setOp)
    {
        Print() << "cmpl %ebx, %eax" << std::endl;
        Print() << "movl $0, %eax" << std::endl;
        Print() << "set" << setOp << " %al" << std::endl;
    }

    void CodeGenVisitor::SimpleBinary(AST::BinaryOpExpr& node)
    {
        node.right->Accept(*this);
        
        Print() << "push %eax" << std::endl;

        node.left->Accept(*this);
        if (node.op == AST::BinaryOp::BITWISE_SHIFT_LEFT
            || node.op == AST::BinaryOp::BITWISE_SHIFT_RIGHT)
        {
            Print() << "pop %ecx" << std::endl;
        }
        else
        {
            Print() << "pop %ebx" << std::endl;
        }
        
        SimpleBinaryOnEaxEbx(node.op);
    }

    void CodeGenVisitor::SimpleBinaryOnEaxEbx(AST::BinaryOp op)
    {
        switch (op)
        {
        case AST::BinaryOp::ADD:
            Print() << "addl %ebx, %eax" << std::endl;
            break;
        case AST::BinaryOp::SUBSTRACT:
            Print() << "subl %ebx, %eax" << std::endl;
            break;
        case AST::BinaryOp::MULTIPLY:
            Print() << "imul %ebx" << std::endl;
            break;
        case AST::BinaryOp::DIVIDE:
            Print() << "cdq" << std::endl;
            Print() << "idivl %ebx" << std::endl;
            break;
        case AST::BinaryOp::MODULO:
            Print() << "cdq" << std::endl;
            Print() << "idivl %ebx" << std::endl;
            Print() << "movl %edx, %eax" << std::endl;
            break;

        case AST::BinaryOp::BITWISE_AND:
            Print() << "and %ebx, %eax" << std::endl;
            break;
        case AST::BinaryOp::BITWISE_XOR:
            Print() << "xor %ebx, %eax" << std::endl;
            break;
        case AST::BinaryOp::BITWISE_OR:
            Print() << "or %ebx, %eax" << std::endl;
            break;

        case AST::BinaryOp::EQUAL:
            BinaryComparison("e");
            break;
        case AST::BinaryOp::NOT_EQUAL:
            BinaryComparison("ne");
            break;

        case AST::BinaryOp::BITWISE_SHIFT_LEFT:
            Print() << "sall %eax" << std::endl;
            break;
        case AST::BinaryOp::BITWISE_SHIFT_RIGHT:
            Print() << "sarl %eax" << std::endl;
            break;

        default:
            break;
        }       
    }

    void CodeGenVisitor::ShortCircuitBinary(AST::BinaryOpExpr& node)
    {
        // TODO: Optimize if left or right are also logical operators, so we don't have to convert %eax to 1 or 0.

        std::string labelRight = GenerateUniqueLabel(); 
        std::string labelEnd = GenerateUniqueLabel(); 

        if (node.op == AST::BinaryOp::LOGIC_AND)
        {
            node.left->Accept(*this);

            Print() << "cmpl $0, %eax" << std::endl;
            Print() << "jne " << labelRight << std::endl;
            Print() << "jmp " << labelEnd << std::endl;

            Print() << labelRight << ":" << std::endl;
            node.right->Accept(*this);
            Print() << "cmpl $0, %eax" << std::endl;
            Print() << "xor %eax, %eax" << std::endl;
            Print() << "setne %al" << std::endl;

            Print() << labelEnd << ":" << std::endl;
        }
        else
        {
            node.left->Accept(*this);

            Print() << "cmpl $0, %eax" << std::endl;
            Print() << "je " << labelRight << std::endl;
            Print() << "movl $1, %eax" << std::endl;
            Print() << "jmp " << labelEnd << std::endl;

            Print() << labelRight << ":" << std::endl;
            node.right->Accept(*this);
            Print() << "cmpl $0, %eax" << std::endl;
            Print() << "xor %eax, %eax" << std::endl;
            Print() << "setne %al" << std::endl;

            Print() << labelEnd << ":" << std::endl;
        }
    }

    void CodeGenVisitor::VisitAssignExpr(AST::AssignExpr& node)
    {
        int stackIndex = MaybeGetVarIndex(node.target);
        // It may be 0, but actually it is possible to generate wrong code. MaybeGetVarIndex will signal error anyways.

        node.expr->Accept(*this);

        if (node.op != AST::AssignOp::SIMPLE_ASSIGN)
        {
            if (node.op == AST::AssignOp::BITWISE_SHIFT_LEFT_ASSIGN
                || node.op == AST::AssignOp::BITWISE_SHIFT_RIGHT_ASSIGN)
            {
                Print() << "movl %eax, %ecx" << std::endl;
            }
            else
            {
                Print() << "movl %eax, %ebx" << std::endl;
            }

            Print() << "movl " << stackIndex << "(%ebp), %eax" << std::endl;

            SimpleBinaryOnEaxEbx(AssignOpToBinaryOp(node.op));
        }

        Print() << "movl %eax, " << stackIndex << "(%ebp)" << std::endl;
    }

    AST::BinaryOp CodeGenVisitor::AssignOpToBinaryOp(AST::AssignOp op) const
    {
        switch (op)
        {
        case AST::AssignOp::ADD_ASSIGN:       return AST::BinaryOp::ADD;
        case AST::AssignOp::SUBSTRACT_ASSIGN: return AST::BinaryOp::SUBSTRACT;
        case AST::AssignOp::MULTIPLY_ASSIGN:  return AST::BinaryOp::MULTIPLY;
        case AST::AssignOp::DIVIDE_ASSIGN:    return AST::BinaryOp::DIVIDE;
        case AST::AssignOp::MODULO_ASSIGN:    return AST::BinaryOp::MODULO;

        case AST::AssignOp::BITWISE_OR_ASSIGN:  return AST::BinaryOp::BITWISE_OR;
        case AST::AssignOp::BITWISE_XOR_ASSIGN: return AST::BinaryOp::BITWISE_XOR;
        case AST::AssignOp::BITWISE_AND_ASSIGN: return AST::BinaryOp::BITWISE_AND;

        case AST::AssignOp::BITWISE_SHIFT_LEFT_ASSIGN:  return AST::BinaryOp::BITWISE_SHIFT_LEFT;
        case AST::AssignOp::BITWISE_SHIFT_RIGHT_ASSIGN: return AST::BinaryOp::BITWISE_SHIFT_RIGHT;

        default:
            Unreachable("AssignToBinaryOp unreachable");
            return AST::BinaryOp::DIVIDE;
        }
    }

    std::string CodeGenVisitor::GenerateUniqueLabel()
    {
        if (labelCount == SIZE_MAX)
        {
            // TODO: ReportError.
            hadError = true;
        }

        return "_label" + std::to_string(labelCount++);
    }

    void CodeGenVisitor::EmitProlog()
    {
        Print() << "pushl %ebx" << std::endl;
        Print() << "movl %esp, %ebp" << std::endl;
        Print() << std::endl;
    }

    void CodeGenVisitor::EmitEpilog()
    {
        Print() << std::endl;
        Print() << "movl %ebp, %esp" << std::endl;
        Print() << "popl %ebx" << std::endl;
    }

    int CodeGenVisitor::MaybeGetVarIndex(std::unique_ptr<AST::Expr>& node)
    {
        AST::VarExpr* var = dynamic_cast<AST::VarExpr*>(node.get());
        if (var == nullptr)
        {
            ReportError(node->pos, "invalid assignment target");
            hadError = true;
            return 0;
        }

        auto iter = vars.find(var->name);
        if (iter == vars.end())
        {
            ReportError(node->pos, "undefined variable");
            hadError = true;
            return 0;
        }

        return iter->second;
    }

    void CodeGenVisitor::VisitVarDeclStmt(AST::VarDeclStmt& node)
    {
        auto iter = vars.find(node.name);
        if (iter != vars.end())
        {
            ReportError(node.pos, "redefinition of variable '" + node.name + "'");
            // TODO: Add 'note: first declared here'.
            return;
        }

        if (node.init != nullptr)
        {
            node.init->Accept(*this);
        }

        Print() << "pushl %eax" << std::endl;

        vars[node.name] = currentStackIndex;
        currentStackIndex -= WORD_SIZE;
    }

    void CodeGenVisitor::VisitExprStmt(AST::ExprStmt& node)
    {
        node.expr->Accept(*this);
    }

    void CodeGenVisitor::VisitVarExpr(AST::VarExpr& node)
    {
        auto iter = vars.find(node.name);
        if (iter == vars.end())
        {
            ReportError(node.pos, "undefined variable '" + node.name + "'");
        }

        Print() << "movl " << iter->second << "(%ebp), %eax" << std::endl;
    }

} // namespace CComp
