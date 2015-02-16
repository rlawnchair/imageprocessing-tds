ROOT=../..

CC=gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -I$(ROOT)/include -L$(ROOT)/lib -lbcl 

%OBJ= test1.o 

all: test1 

test1: test1.o 
	$(CC)  test1.o $(LDFLAGS) -o test1

test1.o: test1.c 

install : all
	cp test1 $(ROOT)/bin/

clean :
	rm -f $(OBJ) test1 


