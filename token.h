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
    Token();
    Token(std::string type_, unsigned line, unsigned column, std::string str);
    ~Token();
    std::string getType() const;
    void setType(std::string type_);
    void setToken(std::string token);
    friend std::ostream& operator<<(std::ostream &os,const Token &token);
};

#endif