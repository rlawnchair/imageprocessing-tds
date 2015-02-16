ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lfftw3 -lm -lbcl 

OBJ= mm_dil mm_ero mm_open mm_close mm_subtract mm_add mm_binarize mm_label

all: mm_dil mm_ero mm_open mm_close mm_subtract mm_add mm_binarize mm_label

mm_label.o : mm_label.c

mm_label : mm_label.o
	$(CC) mm_label.o $(LDFLAGS) -o mm_label

mm_binarize.o : mm_binarize.c

mm_binarize : mm_binarize.o
	$(CC) mm_binarize.o $(LDFLAGS) -o mm_binarize

mm_add.o : mm_add.c

mm_add : mm_add.o
	$(CC) mm_add.o $(LDFLAGS) -o mm_add

mm_subtract.o : mm_subtract.c

mm_subtract : mm_subtract.o
	$(CC) mm_subtract.o $(LDFLAGS) -o mm_subtract

mm_close : mm_close.o mm.o
	$(CC) mm_close.o mm.o $(LDFLAGS) -o mm_close

mm_close.o : mm_close.c

mm_open : mm_open.o mm.o
	$(CC) mm_open.o mm.o $(LDFLAGS) -o mm_open

mm_open.o : mm_open.c

mm_ero : mm_ero.o mm.o
	$(CC) mm_ero.o mm.o $(LDFLAGS) -o mm_ero

mm_ero.o : mm_ero.c

mm_dil : mm_dil.o mm.o
	$(CC) mm_dil.o mm.o $(LDFLAGS) -o mm_dil

mm_dil.o : mm_dil.c

mm.o : mm.c mm.h

clean :
	rm -f $(OBJ)
	rm -f *~


