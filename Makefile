alex: main.o lexer.o token.o
	g++ -o alex main.o lexer.o token.o

main.o: main.cpp
	g++ -c main.cpp

lexer.o 