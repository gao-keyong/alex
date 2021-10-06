objects = alex.o lexer.o token.o

alex: $(objects)
	g++ -o alex $(objects) -g

alex.o: alex.cpp
	g++ -c alex.cpp -g

lexer.o: lexer.cpp lexer.h
	g++ -c lexer.cpp -g

token.o: token.cpp token.h
	g++ -c token.cpp -g

clean: 
	rm $(objects)