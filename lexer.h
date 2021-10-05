#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <deque>
#include "token.h"

bool isalpha_(const char ch)
{
    return isalpha(ch) || ch == '_';
}

class Lexer
{
private:
    std::string fileName_;
    std::ifstream sourceFile_;
    unsigned countChar_;
    unsigned countLine_;
    unsigned countColumn_;
    std::string buffer_;
    Token getNextIdentifier()
    {
        Token token(Identifier, countLine_, countColumn_, "");
        do
        {
            char ch = sourceFile_.peek();
            if (isalpha_(ch))
            {
                eatChar();
            }
            else
                break;
        }
#define KEYWORD(TOK)     \
    if (buffer_ == #TOK) \
        token.setType(Keyword);
#include "def/KEYWORD.def"
#undef KEYWORD
        token.setToken(buffer_);
        return token;
    }

public:
    Lexer(std::string);
    ~Lexer();
    bool lexer();
    void eatChar()
    {
        char ch = sourceFile_.get();
        buffer_.push_back(ch);
        if (ch == '\n')
        {
            countLine_++;
            countColumn_ = 1;
        }
        else
        {
            countColumn_++;
        }
        countChar_++;
    }
};

#endif