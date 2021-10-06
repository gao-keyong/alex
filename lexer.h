#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <deque>
#include "token.h"

bool isalpha_(const char ch);

bool ise(const char ch);

bool issign(const char ch);

class Lexer
{
private:
    std::string fileName_;
    std::ifstream sourceFile_;
    unsigned countChar_;
    unsigned countLine_;
    unsigned countColumn_;
    std::string buffer_;
    void getNextIdentifier(Token &token)
    {
        token.setType("Identifier");
        do
        {
            char ch = peekChar();
            if (isalpha_(ch))
            {
                eatChar();
            }
            else
                break;
        } while (true);
#define KEYWORD(TOK)     \
    if (buffer_ == #TOK) \
        token.setType("Keyword");
#include "def/KEYWORD.def"
#undef KEYWORD
        token.setToken(buffer_);
    }
    void getNextNumerical(Token &token)
    {
        token.setType("Numerical_Constant");
        unsigned state = 2;
        char ch;
        do
        {
            switch (state)
            {
            case 0:
                break;
            case 2:
                ch = peekChar();
                if (isdigit(ch))
                {
                    state = 2;
                    eatChar();
                }
                else if (ch == '.')
                {
                    state = 3;
                    eatChar();
                }
                else if (ise(ch))
                {
                    state = 5;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            case 3:
                ch = peekChar();
                if (isdigit(ch))
                {
                    state = 4;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            case 4:
                ch = peekChar();
                if (isdigit(ch))
                {
                    state = 4;
                    eatChar();
                }
                else if (ise(ch))
                {
                    state = 5;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            case 5:
                ch = peekChar();
                if (isdigit(ch))
                {
                    state = 7;
                    eatChar();
                }
                else if (issign(ch))
                {
                    state = 6;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            case 6:
                ch = peekChar();
                if (isdigit(ch))
                {
                    state = 7;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            case 7:
                ch = peekChar();
                if (isdigit(ch))
                {
                    state = 7;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            default:
                break;
            }
        } while (state != 0);
        token.setToken(buffer_);
    }
    void getCharConstant(Token &token){
        token.setType("Char_Constant");
        unsigned state=1;
        char ch;
        do{
            switch(state){
                case 1:
                    ch=peekChar();
                    if(ch=='\\'){
                        state=3; eatChar();
                    }else if(ch=='\''){
                        state=0; eatChar();
                    }else{
                        state=2; eatChar();
                    }
                    break;
                case 2:
                    ch=peekChar();
                    if(ch=='\''){
                        state=0; eatChar();
                    }else{
                        state=0;
                    }
                    break;
                case 3:
                    state=2; eatChar();
                    break;
                default: break;
            }
        }while(state!=0);
        token.setToken(buffer_);
    }
    void getStringLiteral(Token &token){
        token.setType("String_Literal");
        unsigned state=1;
        char ch;
        do{
            switch(state){
                case 1:
                    ch=peekChar();
                    if(ch=='\\'){
                        state=2; eatChar();
                    }else if(ch=='\"'){
                        state=0; eatChar();
                    }else{
                        state=1; eatChar();
                    }
                    break;
                case 2:
                    state=1; eatChar();
                    break;
                default: break;
            }
        }while(state!=0);
        token.setToken(buffer_);
    }
public:
    Lexer(std::string fileName);
    ~Lexer();
    bool lexer();
    char eatChar()
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
        return ch;
    }
    char peekChar()
    {
        return sourceFile_.peek();
    }
};

#endif