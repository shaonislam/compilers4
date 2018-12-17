CC=g++
CFLAGS= -Wall -I.
DEPS = token.hpp scanner.hpp  staticSemantics.hpp parser.hpp
OBJ = main.o token.o scanner.o  staticSemantics.o parser.o
TARGET = comp

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *.asm $(TARGET)
