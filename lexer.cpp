#include "lexer.h"
#include <cassert>
#include <cctype>

Lexer::Lexer(std::string fileName) : fileName_(fileName), countChar_(0), countLine_(1), countColumn_(1)
{
    sourceFile_.open(fileName);
    assert(sourceFile_.is_open());
    buffer_.clear();
}

bool Lexer::lexer()
{
    char ch = sourceFile_.get();
    while (!(sourceFile_.eof() && buffer_.empty()))
    {
        Token token;
        if (isalpha_(ch))
        { // keyword or identifier
            buffer_.push_back(ch);
            token = getNextIdentifier();
        }
        std::cout<<token<<std::endl;
    }
}