#ifndef CCOMP_AST_DECL_HPP
#define CCOMP_AST_DECL_HPP

#include <memory>
#include <vector>

#include "Node.hpp"
#include "Forwards.hpp"
#include "Visitor.hpp"

namespace CComp
{
    namespace AST
    {
        class Decl : public Node
        {
        public:
            Decl(FilePosition pos)
                : Node(pos)
            {}
        };
        
        class TypeAndNameDecl : public Decl // Can be used as parameter, part of variable declaration/definition.
        {
        public:
            TypeAndNameDecl(FilePosition pos, std::unique_ptr<Type> type, const std::string& name)
                : Decl(pos),
                  type(std::move(type)), name(name)
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitTypeAndNameDecl(*this);
            }

            std::unique_ptr<Type> type;
            std::string name;
        };

        using Parameter = TypeAndNameDecl;
        
        class FunctionDecl : public Decl
        {
        public:
            FunctionDecl(FilePosition pos,
                         std::unique_ptr<Type> returnType,
                         const std::string& str,
                         std::vector<std::unique_ptr<Parameter>> parameters,
                         std::unique_ptr<BlockStmt> body)
                : Decl(pos),
                  returnType(std::move(returnType)),
                  str(str),
                  parameters(std::move(parameters)),
                  body(std::move(body))
            {}
            
            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitFunctionDecl(*this);
            }

            std::unique_ptr<Type> returnType;
            std::string str;
            std::vector<std::unique_ptr<Parameter>> parameters;
            std::unique_ptr<BlockStmt> body;
        };
    } // namespace AST
} // namespace CComp

#endif // CCOMP_AST_DECL_HPP
