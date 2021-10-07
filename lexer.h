#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <deque>
#include "token.h"

// 判断字符是否为字母或'_'
bool isalpha_(const char ch);

// 判断字符是否为字母、数字或'_'
bool isalphadigit_(const char ch);

// 判断字符是否为'e'或'E'
bool ise(const char ch);

// 判断字符是否为'+'或'-'
bool issign(const char ch);

class Lexer
{
private:
    std::string fileName_;      // 词法分析器分析的源程序文件名
    std::ifstream sourceFile_;  // 词法分析器分析的源程序文件输入流
    unsigned countChar_;        // 当前读入字符
    unsigned countLine_;        // 当前所在行
    unsigned countColumn_;      // 当前所在列
// 定义当前已分析出的各类token的数目
#define TOKEN_TYPE(TYP) unsigned count_##TYP;
#include "def/TOKEN_TYPE.def"
#undef TOKEN_TYPE
    std::string buffer_;        // 缓冲区
    // 读取当前标识符
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
// 特判该token是否为关键词Keyword
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
    // 读取当前数值常量
    void getNextNumerical(Token &token)
    {
        token.setType("Numerical_Constant");
        eatChar();
        // 识别数值常量的自动机，对应的状态转移图为Graph_1.jpg
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
    // 读取当前字符常量
    void getCharConstant(Token &token)
    {
        token.setType("Char_Constant");
        eatChar();
        // 识别字符常量的自动机
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
    // 读取当前字符串常量
    void getStringLiteral(Token &token)
    {
        token.setType("String_Literal");
        eatChar();
        // 识别字符串常量的自动机
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
    // 词法分析器过程
    bool lexer();
    // 读取下一个字符，并将当前光标移至该字符之后
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
    // 读取下一个字符
    char peekChar()
    {
        return sourceFile_.peek();
    }
    void printStat();
};

#endif