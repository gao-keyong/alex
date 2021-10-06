#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <deque>
#include "token.h"

bool isalpha_(const char ch);

bool isalphadigit_(const char ch);

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
#define TOKEN_TYPE(TYP) unsigned count_##TYP;
#include "def/TOKEN_TYPE.def"
#undef TOKEN_TYPE
    std::string buffer_;
    void getNextIdentifier(Token &token)
    {
        token.setType("Identifier");
        eatChar();
        do
        {
            char ch = peekChar();
            if (isalphadigit_(ch))
            {
                eatChar();
            }
            else
                break;
        } while (true);
#define KEYWORD(TOK)     \
    if (buffer_ == #TOK) \
        token.setType("Keyword"), count_Keyword++;
#include "def/KEYWORD.def"
#undef KEYWORD
        if (token.getType() == "Identifier")
        {
            count_Identifier++;
        }
        token.setToken(buffer_);
    }
    void getNextNumerical(Token &token)
    {
        token.setType("Numerical_Constant");
        eatChar();
        unsigned state = 2;
        char ch;
        do
        {
            switch (state)
            {
            case 0:
                break;
            case 1:
                ch = peekChar();
                if (isalpha_(ch))
                {
                    state = 1;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
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
                else if (isalpha_(ch))
                {
                    token.setType("Error");
                    token.setError("illegal name");
                    count_Error++;
                    state = 1;
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
        if (token.getType() == "Numerical_Constant")
        {
            count_Numerical_Constant++;
        }
    }
    void getCharConstant(Token &token)
    {
        token.setType("Char_Constant");
        eatChar();
        unsigned state = 1;
        char ch;
        do
        {
            switch (state)
            {
            case 1:
                ch = peekChar();
                if (ch == '\\')
                {
                    state = 3;
                    eatChar();
                }
                else if (ch == '\'')
                {
                    state = 0;
                    eatChar();
                }
                else
                {
                    state = 2;
                    eatChar();
                }
                break;
            case 2:
                ch = peekChar();
                if (ch == '\'')
                {
                    state = 0;
                    eatChar();
                }
                else
                {
                    state = 0;
                }
                break;
            case 3:
                state = 2;
                eatChar();
                break;
            default:
                break;
            }
        } while (state != 0);
        token.setToken(buffer_);
        if (token.getType() == "Char_Constant")
        {
            count_Char_Constant++;
        }
    }
    void getStringLiteral(Token &token)
    {
        token.setType("String_Literal");
        eatChar();
        unsigned state = 1;
        char ch;
        do
        {
            switch (state)
            {
            case 1:
                ch = peekChar();
                if (ch == '\\')
                {
                    state = 2;
                    eatChar();
                }
                else if (ch == '\"')
                {
                    state = 0;
                    eatChar();
                }
                else if (ch == '\n')
                {
                    token.setType("Error");
                    token.setError("unclosed string");
                    count_Error++;
                    state = 0;
                }
                else
                {
                    state = 1;
                    eatChar();
                }
                break;
            case 2:
                state = 1;
                eatChar();
                break;
            default:
                break;
            }
        } while (state != 0);
        token.setToken(buffer_);
        if (token.getType() == "String_Literal")
        {
            count_String_Literal++;
        }
    }

public:
    Lexer(std::string fileName);
    ~Lexer();
    bool lexer();
    char eatChar()
    {
        char ch = sourceFile_.get();
        if (ch >= 0)
            buffer_.push_back(ch);
        else
        {
            return 0;
        }
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
    void printStat();
};

#endif