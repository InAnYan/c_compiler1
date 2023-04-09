#ifndef CCOMP_TYPE_HPP
#define CCOMP_TYPE_HPP

#include "Node.hpp"
#include "Visitor.hpp"

namespace CComp
{
    namespace AST
    {
        class Type : public Node
        {
        public:
            Type(FilePosition pos)
                : Node(pos)
            {}

        }; // class Type

        class BasicType : public Type
        {
        public:
            BasicType(FilePosition pos, const std::string& name)
                : Type(pos),
                  name(name)
            {}

            virtual void Accept(Visitor& visitor)
            {
                visitor.VisitBasicType(*this);
            }

            std::string name;
        }; // class BasicType
   } // namespace AST
} // namespace CComp

#endif // CCOMP_TYPE_HPP
