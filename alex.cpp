#include <iostream>
#include "lexer.h"

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        Lexer lexer(argv[1]);
        bool noerr = lexer.lexer();
    }
    else
    {
        std::cout << "Usage: alex <file name>" << std::endl;
    }
    return 0;
}