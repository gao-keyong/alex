#include "token.h"

void Token::setType(std::string type_)
{
    this->type_ = type_;
}

void Token::setToken(std::string token)
{
    token_ = token;
}

std::ostream& operator<<(std::ostream &os,const Token &token){
    if(token.type_!="Unknown"){
        os<<token.line_<<':'<<token.column_<<": ["<<token.type_<<": "<<token.token_<<"]";
    }else{
        os<<token.line_<<":"<<token.column_<<": ["<<token.type_<<": "<<token.token_<<"]";
    }
    return os;
}

Token::Token(std::string type_, unsigned line, unsigned column, std::string token) : type_(type_), line_(line), column_(column), token_(token)
{
}

Token::Token(){
}

Token::~Token()
{
}
