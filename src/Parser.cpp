#include "Parser.hpp"

#include "Reporter.hpp"
#include "AST/AST.hpp"
#include "Unreachable.hpp"

namespace CComp
{
    Parser::Parser(Scanner& scanner)
        : m_Scanner(scanner)
    {}

    std::vector<std::unique_ptr<AST::Decl>> Parser::Parse()
    {
        Advance();

        std::vector<std::unique_ptr<AST::Decl>> res;

        while (!IsAtEnd())
        {
            res.push_back(ParseDecl());
        }

        if (!m_HadError)
        {
            return res;
        }
        else
        {
            return {};
        }
    }

    std::unique_ptr<AST::Decl> Parser::ParseDecl()
    {
        std::unique_ptr<AST::Type> returnType = ParseType();
        Consume(TokenType::IDENTIFIER, "expected function name");
        Token name = m_Current; 
        Consume(TokenType::LEFT_PAREN, "expected '(' after function name");
        Consume(TokenType::RIGHT_PAREN, "expected ')' after function parameters");
        Consume(TokenType::LEFT_BRACKET, "expected '{'");
        std::unique_ptr<AST::BlockStmt> body = ParseBlock();
        return std::make_unique<AST::FunctionDecl>(name.pos,
                                                   std::move(returnType),
                                                   name.str,
                                                   std::vector<std::unique_ptr<AST::Parameter>>(),
                                                   std::move(body));
    }

    std::unique_ptr<AST::Stmt> Parser::ParseStmt()
    {
        if (Match(TokenType::RETURN))
        {
            return ParseReturnStmt();
        }
        else if (Match(TokenType::INT))
        {
            return ParseVarDeclStmt();
        }

        return ParseExprStmt();
    }

    std::unique_ptr<AST::ExprStmt> Parser::ParseExprStmt()
    {
        std::unique_ptr<AST::Expr> expr = ParseExpr();
        Consume(TokenType::SEMICOLON, "expected ';' after expression statement");
        return std::make_unique<AST::ExprStmt>(expr->pos, std::move(expr));
    }

    std::unique_ptr<AST::VarDeclStmt> Parser::ParseVarDeclStmt()
    {
        std::unique_ptr<AST::Type> type = std::make_unique<AST::BasicType>(m_Current.pos, m_Current.str);

        Consume(TokenType::IDENTIFIER, "expected variable name");
        Token name = m_Current;

        std::unique_ptr<AST::Expr> init = nullptr;
        if (Match(TokenType::EQUAL))
        {
            init = ParseExpr();
        }

        Consume(TokenType::SEMICOLON, "expected ';' after var declaration statement");

        return std::make_unique<AST::VarDeclStmt>(name.pos, std::move(type), name.str, std::move(init));
    }

    std::unique_ptr<AST::Expr> Parser::ParseExpr()
    {
        return ParseCommaExpr();
    }
    
    std::unique_ptr<AST::Expr> Parser::ParseCommaExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseAssignmentExpr();

        while (Match(TokenType::COMMA))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseAssignmentExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }
    
    std::unique_ptr<AST::Expr> Parser::ParseAssignmentExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseLogicalOrExpr();

        while (Match(TokenType::EQUAL, TokenType::PLUS_EQUAL,
                     TokenType::MINUS_EQUAL, TokenType::STAR_EQUAL,
                     TokenType::SLASH_EQUAL, TokenType::PERCENT_EQUAL,
                     TokenType::LESS_LESS_EQUAL, TokenType::GREATER_GREATER_EQUAL,
                     TokenType::AMPERSAND_EQUAL, TokenType::UP_ARROW_EQUAL,
                     TokenType::BAR_EQUAL))
        {
            AST::AssignOp op = TokenTypeToAssignOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseLogicalOrExpr();

            left = std::make_unique<AST::AssignExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseLogicalOrExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseLogicalAndExpr();

        while (Match(TokenType::BAR_BAR))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseLogicalAndExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }
    
    std::unique_ptr<AST::Expr> Parser::ParseLogicalAndExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseBitwiseOrExpr();

        while (Match(TokenType::AMPERSAND_AMPERSAND))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseBitwiseOrExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseBitwiseOrExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseBitwiseXorExpr();

        while (Match(TokenType::BAR))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseBitwiseXorExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseBitwiseXorExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseBitwiseAndExpr();

        while (Match(TokenType::UP_ARROW))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseBitwiseAndExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseBitwiseAndExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseBitwiseShiftExpr();

        while (Match(TokenType::AMPERSAND))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseBitwiseShiftExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }
    
    std::unique_ptr<AST::Expr> Parser::ParseBitwiseShiftExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseEqualityExpr();

        while (Match(TokenType::GREATER_GREATER, TokenType::LESS_LESS))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseEqualityExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseEqualityExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseComparisonExpr();

        while (Match(TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseComparisonExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseComparisonExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseTermExpr();

        while (Match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseTermExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<AST::Expr> Parser::ParseTermExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseFactorExpr();

        while (Match(TokenType::PLUS, TokenType::MINUS))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseFactorExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    } 
    
    std::unique_ptr<AST::Expr> Parser::ParseFactorExpr()
    {
        std::unique_ptr<AST::Expr> left = ParseUnaryExpr();

        while (Match(TokenType::STAR, TokenType::SLASH, TokenType::PERCENT))
        {
            AST::BinaryOp op = TokenTypeToBinaryOp(m_Current.type);
            FilePosition pos = m_Current.pos;

            std::unique_ptr<AST::Expr> right = ParseUnaryExpr();

            left = std::make_unique<AST::BinaryOpExpr>(pos, op, std::move(left), std::move(right));
        }

        return left;
    } 

    std::unique_ptr<AST::Expr> Parser::ParseUnaryExpr()
    {
        if (Match(TokenType::BANG) || Match(TokenType::TILDA)
            || Match(TokenType::MINUS) || Match(TokenType::PLUS_PLUS)
            || Match(TokenType::MINUS_MINUS))
        {
            FilePosition pos = m_Current.pos;
            AST::UnaryOp op = TokenTypeToUnaryOp(m_Current.type, true);
            std::unique_ptr<AST::Expr> expr = ParseUnaryExpr();

            return std::make_unique<AST::UnaryOpExpr>(pos, op, std::move(expr));
        }
        else if (Match(TokenType::PLUS))
        {
            return ParseUnaryExpr();
        }
        else
        {
            return ParsePrimaryExpr();
        }

        ErrorAtNext("expected expression");
        return nullptr;
    }

    std::unique_ptr<AST::Expr> Parser::ParsePrimaryExpr()
    {
        std::unique_ptr<AST::Expr> expr = nullptr;

        if (Match(TokenType::LEFT_PAREN))
        {
            expr = ParseExpr();
            Consume(TokenType::RIGHT_PAREN, "expected ')'");
        }
        else if (Match(TokenType::INTEGER_NUMBER))
        {
            expr = std::make_unique<AST::IntegerNumberExpr>(m_Current.pos, m_Current.str);
        }
        else if (Match(TokenType::IDENTIFIER))
        {
            expr = std::make_unique<AST::VarExpr>(m_Current.pos, m_Current.str);
        }

        if (expr == nullptr)
        {
            ErrorAtNext("expected expression");
            return nullptr;
        }

        while (Match(TokenType::PLUS_PLUS, TokenType::MINUS_MINUS))
        {
            expr = std::make_unique<AST::UnaryOpExpr>(m_Current.pos,
                                                      TokenTypeToUnaryOp(m_Current.type, false),
                                                      std::move(expr));
        }

        return expr;
    }

    AST::UnaryOp Parser::TokenTypeToUnaryOp(TokenType type, bool isPrefix) const
    {
        switch (type)
        {
        case TokenType::BANG:  return AST::UnaryOp::NOT;
        case TokenType::TILDA: return AST::UnaryOp::BITWISE_COMPLEMENT;
        case TokenType::MINUS: return AST::UnaryOp::NEGATION;

        case TokenType::PLUS_PLUS: return isPrefix
                ? AST::UnaryOp::PRE_INCREMENT
                : AST::UnaryOp::POST_INCREMENT;

        case TokenType::MINUS_MINUS: return isPrefix
                ? AST::UnaryOp::PRE_DECREMENT
                : AST::UnaryOp::POST_DECREMENT;

        default:
            Unreachable("TokenTypeToUnaryOp unreachable");
            return AST::UnaryOp::NEGATION;
        }
    }

    AST::BinaryOp Parser::TokenTypeToBinaryOp(TokenType type) const
    {
        switch (type)
        {
        case TokenType::COMMA:                 return AST::BinaryOp::COMMA;
        case TokenType::PLUS:                  return AST::BinaryOp::ADD;
        case TokenType::MINUS:                 return AST::BinaryOp::SUBSTRACT;
        case TokenType::STAR:                  return AST::BinaryOp::MULTIPLY;
        case TokenType::SLASH:                 return AST::BinaryOp::DIVIDE;
        case TokenType::BAR:                   return AST::BinaryOp::BITWISE_OR;
        case TokenType::AMPERSAND:             return AST::BinaryOp::BITWISE_AND;
        case TokenType::BAR_BAR:               return AST::BinaryOp::LOGIC_OR;
        case TokenType::AMPERSAND_AMPERSAND:   return AST::BinaryOp::LOGIC_AND;
        case TokenType::GREATER:               return AST::BinaryOp::GREATER;
        case TokenType::LESS:                  return AST::BinaryOp::LESS;
        case TokenType::GREATER_EQUAL:         return AST::BinaryOp::GREATER_EQUAL;
        case TokenType::LESS_EQUAL:            return AST::BinaryOp::LESS_EQUAL;
        case TokenType::UP_ARROW:              return AST::BinaryOp::BITWISE_XOR;
        case TokenType::EQUAL_EQUAL:           return AST::BinaryOp::EQUAL;
        case TokenType::BANG_EQUAL:            return AST::BinaryOp::NOT_EQUAL;
        case TokenType::LESS_LESS:             return AST::BinaryOp::BITWISE_SHIFT_LEFT;
        case TokenType::GREATER_GREATER:       return AST::BinaryOp::BITWISE_SHIFT_RIGHT;
        case TokenType::PERCENT:               return AST::BinaryOp::MODULO;
        default:
            Unreachable("TokenTypeToBinaryOp unreachable");
            return AST::BinaryOp::ADD;
        }
    }
    
    AST::AssignOp Parser::TokenTypeToAssignOp(TokenType type) const
    {
        switch (type)
        {
        case TokenType::PLUS_EQUAL:            return AST::AssignOp::ADD_ASSIGN;
        case TokenType::MINUS_EQUAL:           return AST::AssignOp::SUBSTRACT_ASSIGN;
        case TokenType::STAR_EQUAL:            return AST::AssignOp::MULTIPLY_ASSIGN;
        case TokenType::SLASH_EQUAL:           return AST::AssignOp::DIVIDE_ASSIGN;
        case TokenType::BAR_EQUAL:             return AST::AssignOp::BITWISE_OR_ASSIGN;
        case TokenType::AMPERSAND_EQUAL:       return AST::AssignOp::BITWISE_AND_ASSIGN;
        case TokenType::UP_ARROW_EQUAL:        return AST::AssignOp::BITWISE_XOR_ASSIGN;
        case TokenType::EQUAL:                 return AST::AssignOp::SIMPLE_ASSIGN;
        case TokenType::LESS_LESS_EQUAL:       return AST::AssignOp::BITWISE_SHIFT_LEFT_ASSIGN;
        case TokenType::GREATER_GREATER_EQUAL: return AST::AssignOp::BITWISE_SHIFT_RIGHT_ASSIGN;
        case TokenType::PERCENT_EQUAL:         return AST::AssignOp::MODULO_ASSIGN;
        default:
            Unreachable("TokenTypeToAssignOp unreachable");
            return AST::AssignOp::SIMPLE_ASSIGN;
        }
    }

    std::unique_ptr<AST::Type> Parser::ParseType()
    {
        if (Match(TokenType::INT))
        {
            return std::make_unique<AST::BasicType>(m_Current.pos, m_Current.str);
        }

        ErrorAtNext("expected type");
        return nullptr;
    }

    std::unique_ptr<AST::BlockStmt> Parser::ParseBlock()
    {
        std::vector<std::unique_ptr<AST::Stmt>> stmts;
        FilePosition pos = m_Current.pos;

        while (!IsAtEnd() && !Check(TokenType::RIGHT_BRACKET))
        {
            stmts.push_back(ParseStmt());

            if (m_PanicMode == true)
            {
                Synchronize();
            }
        }

        if (IsAtEnd())
        {
            ErrorAtNext("expected '}'");
            return nullptr;
        }

        Advance(); // Consume '}'.

        return std::make_unique<AST::BlockStmt>(pos, std::move(stmts));
    }

    std::unique_ptr<AST::ReturnStmt> Parser::ParseReturnStmt()
    {
        FilePosition pos = m_Current.pos;

        std::unique_ptr<AST::Expr> expr = ParseExpr();
        Consume(TokenType::SEMICOLON, "expected ';' after return statement");

        return std::make_unique<AST::ReturnStmt>(pos, std::move(expr));
    }

    void Parser::Consume(TokenType type, const std::string& errorMsg)
    {
        if (!Check(type))
        {
            m_HadError = true;
            m_PanicMode = true;
            ErrorAtNext(errorMsg);
            return;
        }

        Advance();
    }

    bool Parser::Match(TokenType type)
    {
        if (Check(type))
        {
            Advance();
            return true;
        }

        return false;
    }

    void Parser::ErrorAtCurrent(const std::string& errorMsg)
    {
        ReportError(m_Current.pos, errorMsg);
    }

    void Parser::ErrorAtNext(const std::string& errorMsg)
    {
        ReportError(m_Next.pos, errorMsg);
    }

    void Parser::Synchronize()
    {
        m_PanicMode = false;
        
        while (true)
        {
            Advance();

            if (m_Current.type == TokenType::SEMICOLON)
            {
                break;
            }

            switch (m_Next.type)
            {
            case TokenType::INT:
            case TokenType::RETURN:
                return;
            default:
                continue;
            }
        }
    }

    bool Parser::IsAtEnd() const
    {
        return m_Next.type == TokenType::END_OF_FILE;
    }

    bool Parser::Check(TokenType type) const
    {
        return m_Next.type == type;
    }

    void Parser::Advance()
    {
        m_Current = m_Next;

        while (true)
        {
            m_Next = m_Scanner.NextToken();

            if (m_Next.type == TokenType::ERROR)
            {
                m_HadError = true;
                m_PanicMode = true;
                ReportError(m_Next.pos, m_Next.str);
                continue;
            }

            break;
        }
    }
} // namespace CComp
