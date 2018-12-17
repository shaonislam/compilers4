CC=g++
CFLAGS=-I.
DEPS = token.hpp scanner.hpp parser.hpp testTree.hpp staticSemantics.hpp
OBJ = main.o token.o scanner.o parser.o testTree.o staticSemantics.o
TARGET = comp

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *.asm $(TARGET)
