.SUFFIXES: .cpp

CC = g++
CFLAGS = -g -I/usr/local/include -std=c++11
LDIR = -L/usr/local/lib
LIBS = -lgmpxx -lgmp

OBJ = modTests.o ModularArithmetic.o
OBJ2 = rsaTests.o RSA.o ModularArithmetic.o
OBJ3 = main.o RSA.o ModularArithmetic.o

all: modtests rsatests main

modtests: $(OBJ)
	$(CC) $(CFLAGS) -o runModTests $(OBJ) $(LDIR) $(LIBS)

rsatests: $(OBJ2)
	$(CC) $(CFLAGS) -o runRSATests $(OBJ2) $(LDIR) $(LIBS)
main: $(OBJ3)
	$(CC) $(CFLAGS) -o main $(OBJ3) $(LDIR) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $?

clean:
	rm -f runModTests runRSATests $(OBJ) $(OBJ2) $(OBJ3)
