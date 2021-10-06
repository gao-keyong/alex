#include "token.h"

std::string Token::getType() const
{
    return type_;
}

void Token::setType(std::string type_)
{
    this->type_ = type_;
}

void Token::setToken(std::string token)
{
    token_ = token;
}

void Token::setError(std::string error)
{
    error_ = error;
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    if (token.type_ != "Error")
    {
        os << token.line_ << ':' << token.column_ << ": [" << token.type_ << ": " << token.token_ << "]";
    }
    else
    {
        os << "\033[1m" << token.line_ << ":" << token.column_ << ": "
           << "\033[31mError: \033[32m" << token.error_ << "\033[39m " << token.token_ << "\033[0m";
    }
    return os;
}

Token::Token(std::string type_, unsigned line, unsigned column, std::string token) : type_(type_), line_(line), column_(column), token_(token)
{
}

Token::Token()
{
}

Token::~Token()
{
}
