### __PROJECT_MAKEFILE__ ###

CC = g++
# compiling in 64 bits
CFLAGS = -m64 -std=c++17
LDFLAGS = -m64 -L"./lib"
EXEC = bin/output.exe

all : $(EXEC)

$(EXEC): main.o Encoder.o Model.o
		$(CC) -o $(EXEC) $^ $(LDFLAGS)

main.o:	src/main.cpp
		$(CC) -c $< $(CFLAGS)

Encoder.o: src/Encoder.cpp
		$(CC) -c $< $(CFLAGS)

Model.o: src/Model.cpp
		$(CC) -c $< $(CFLAGS)

clean:
		del *.o

mrproper: clean 
		del -f $(EXEC)