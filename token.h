#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>

class Token
{
private:
    std::string type_;
    unsigned line_;
    unsigned column_;
    std::string token_;

public:
    Token(std::string type_, unsigned line, unsigned column, std::string str);
    ~Token();
    void setType(std::string type_);
    void setToken(std::string token);
    friend std::ostream& operator<<(std::ostream &os,const Token &token);
};

Token::Token(std::string type_, unsigned line, unsigned column, std::string token) : type_(type_), line_(line), column_(column), token_(token)
{
}

Token::~Token()
{
}

#endif