#include "lexer.h"
#include <cassert>
#include <cctype>
#include <iostream>

bool isalpha_(const char ch)
{
    return isalpha(ch) || ch == '_';
}

bool isalphadigit_(const char ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_';
}

bool isws(const char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\n' || ch <= 0;
}

bool ise(const char ch)
{
    return ch == 'e' || ch == 'E';
}

bool issign(const char ch)
{
    return ch == '+' || ch == '-';
}

Lexer::Lexer(std::string fileName) : fileName_(fileName), countChar_(0), countLine_(1), countColumn_(1)
{
    sourceFile_.open(fileName);
    assert(sourceFile_.is_open());
#define TOKEN_TYPE(TYP) count_##TYP = 0;
#include "def/TOKEN_TYPE.def"
#undef TOKEN_TYPE
    buffer_.clear();
}

Lexer::~Lexer()
{
}

bool Lexer::lexer()
{
    bool res = true;
    while (!(sourceFile_.eof() && buffer_.empty()))
    {
        char ch = peekChar();
        if (isalpha_(ch))
        { // keyword or identifier
            Token token("Identifier", countLine_, countColumn_, "");
            getNextIdentifier(token);
            std::cout << token << std::endl;
        }
        else if (isdigit(ch))
        { // numerical constant
            Token token("Numerical_Constant", countLine_, countColumn_, "");
            getNextNumerical(token);
            std::cout << token << std::endl;
        }
        else if (ch == '\'')
        {
            Token token("Char_Constant", countLine_, countColumn_, "");
            getCharConstant(token);
            std::cout << token << std::endl;
        }
        else if (ch == '\"')
        {
            Token token("String_Literal", countLine_, countColumn_, "");
            getStringLiteral(token);
            std::cout << token << std::endl;
        }
        else
        {
            if (!isws(ch))
            {
                Token token("Punctuator", countLine_, countColumn_, "");
                eatChar();
                bool has_error = false;
                bool is_comment = false;
                switch (ch)
                {
                case '.':
                    ch = peekChar();
                    if (ch == '.')
                    {
                        eatChar();
                        if ((ch = peekChar()) == '.')
                        {
                            eatChar();
                        }
                        else
                        {
                            has_error = true;
                            token.setType("Error");
                        }
                    }
                    break;
                case '>':
                    ch = peekChar();
                    if (ch == '>')
                    {
                        eatChar();
                        ch = peekChar();
                        if (ch == '=')
                        {
                            eatChar();
                        }
                    }
                    else if (ch == '=')
                        eatChar();
                    break;
                case '<':
                    ch = peekChar();
                    if (ch == '<')
                    {
                        eatChar();
                        if ((ch = peekChar()) == '=')
                        {
                            eatChar();
                        }
                    }
                    else if (ch == '=')
                        eatChar();
                    break;
                case '+':
                    ch = peekChar();
                    if (ch == '=' || ch == '+')
                        eatChar();
                    break;
                case '-':
                    ch = peekChar();
                    if (ch == '=' || ch == '-' || ch == '>')
                        eatChar();
                    break;
                case '&':
                    ch = peekChar();
                    if (ch == '=' || ch == '&')
                        eatChar();
                    break;
                case '|':
                    ch = peekChar();
                    if (ch == '=' || ch == '|')
                        eatChar();
                    break;
                case '*':
                case '%':
                case '^':
                case '=':
                case '!':
                    ch = peekChar();
                    if (ch == '=')
                        eatChar();
                    break;
                case ';':
                case '{':
                case '}':
                case ',':
                case ':':
                case '(':
                case ')':
                case '[':
                case ']':
                case '~':
                case '?':
                    break;
                case '#':
                    is_comment = true;
                    do
                    {
                        ch = eatChar();
                    } while (ch != '\n');
                    break;
                case '/':
                    ch = peekChar();
                    if (ch == '*' || ch == '/')
                    {
                        is_comment = true;
                        // 识别注释的自动机，其状态转移图为Graph_2.jpg
                        unsigned state = 1;
                        do
                        {
                            ch = peekChar();
                            if (ch <= 0)
                                state = 5;
                            switch (state)
                            {
                            case 1:
                                if (ch == '*')
                                {
                                    state = 2;
                                    eatChar();
                                }
                                else if (ch == '/')
                                {
                                    state = 4;
                                    eatChar();
                                }
                                break;
                            case 2:
                                if (ch == '*')
                                {
                                    state = 3;
                                    eatChar();
                                }
                                else
                                {
                                    state = 2;
                                    eatChar();
                                }
                                break;
                            case 3:
                                if (ch == '*')
                                {
                                    state = 3;
                                    eatChar();
                                }
                                else if (ch == '/')
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
                            case 4:
                                if (ch == '\n')
                                {
                                    state = 0;
                                    eatChar();
                                }
                                else
                                {
                                    state = 4;
                                    eatChar();
                                }
                                break;
                            default:
                                state = 0;
                                token.setType("Error");
                                token.setError("unterminated comment");
                                count_Error++;
                                break;
                            }
                        } while (state != 0);
                    }
                    else if (ch == '=')
                        eatChar();
                    break;
                default:
                    token.setType("Error");
                    token.setError("unexpected character");
                    count_Error++;
                    token.setToken(buffer_);
                    break;
                }
                res &= !has_error;
                if (token.getType() == "Punctuator")
                {
                    count_Punctuator++;
                }
                token.setToken(buffer_);
                std::cout << token << std::endl;
            }
            else
            {
                eatChar();
            }
        }
        buffer_.clear();
    }
    return res;
}

void Lexer::printStat()
{
    std::cout << std::endl;
    std::cout << "Total characters:\t" << countChar_ << std::endl;
    std::cout << "Total lines:\t\t" << countLine_ << std::endl
              << std::endl;
#define TOKEN_TYPE(TYP) std::cout << #TYP << ":\t" << count_##TYP << std::endl;
#include "def/TOKEN_TYPE.def"
#undef TOKEN_TYPE
}