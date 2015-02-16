ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lfftw3 -lm -lbcl 

OBJ= color2mean td-app rotate mm_binarize mm_label mm_close

all: color2mean td-app rotate mm_binarize mm_label mm_close

mm.o : mm.c mm.h

mm_label.o : mm_label.c

mm_label : mm_label.o
	$(CC) mm_label.o $(LDFLAGS) -o mm_label

mm_close.o : mm_close.c

mm_close : mm_close.o mm.o
	$(CC) mm_close.o mm.o $(LDFLAGS) -o mm_close

mm_binarize.o : mm_binarize.c

mm_binarize : mm_binarize.o
	$(CC) mm_binarize.o $(LDFLAGS) -o mm_binarize

color2mean.o : color2mean.c

color2mean : color2mean.o
	$(CC) color2mean.o $(LDFLAGS) -o color2mean

td-app.o : td-app.c

td-app : td-app.o
	$(CC) td-app.o $(LDFLAGS) -o td-app

rotate.o : rotate.c

rotate : rotate.o
	$(CC) rotate.o $(LDFLAGS) -o rotate

clean :
	rm -f $(OBJ) color2mean.o td-app.o rotate.o mm_binarize.o mm.o mm_label.o mm_close
	rm -f *~


