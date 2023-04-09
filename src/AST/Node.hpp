#ifndef CCOMP_AST_NODE_HPP
#define CCOMP_AST_NODE_HPP

#include "../File/FilePosition.hpp"

namespace CComp
{
    namespace AST
    {
        class Visitor;

        class Node
        {
        public:
            Node(FilePosition pos)
                : m_Pos(pos)
            {}

            virtual ~Node() = default;

            virtual void Accept(Visitor& visitor) = 0;

            FilePosition m_Pos;
        };
    } // namespace AST
} // namespace CComp

#endif // CCOMP_AST_NODE_HPP
