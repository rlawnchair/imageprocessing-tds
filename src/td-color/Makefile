ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lfftw3 -lm -lbcl 

OBJ= color_transfer color_transfer.o

all: color_transfer

color_transfer : color_transfer.o
	$(CC) color_transfer.o $(LDFLAGS) -o color_transfer

mm_close.o : color_transfer.c

clean :
	rm -f $(OBJ)
	rm -f *~


