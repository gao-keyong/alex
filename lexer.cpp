#include "lexer.h"
#include <cassert>
#include <cctype>
#include <iostream>

bool isalpha_(const char ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_';
}

bool isws(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f';
}

bool ise(char ch)
{
    return ch == 'e' || ch == 'E';
}

bool issign(char ch)
{
    return ch == '+' || ch == '-';
}

Lexer::Lexer(std::string fileName) : fileName_(fileName), countChar_(0), countLine_(1), countColumn_(1)
{
    sourceFile_.open(fileName);
    assert(sourceFile_.is_open());
    buffer_.clear();
}

Lexer::~Lexer()
{
}

bool Lexer::lexer()
{
    while (!(sourceFile_.eof() && buffer_.empty()))
    {
        char ch = eatChar();
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
        }else if(ch=='\''){
            Token token("Char_Constant", countLine_,countColumn_,"");
            getCharConstant(token);
            std::cout<<token<<std::endl;
        }else if(ch=='\"'){
            Token token("String_Literal", countLine_,countColumn_,"");
            getStringLiteral(token);
            std::cout<<token<<std::endl;
        }
        buffer_.clear();
    }
    return true;
}