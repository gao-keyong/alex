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
    void getNextIdentifier(Token &token);
    // 读取当前数值常量
    void getNextNumerical(Token &token);
    // 读取当前字符常量
    void getCharConstant(Token &token);
    // 读取当前字符串常量
    void getStringLiteral(Token &token);

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