ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lbcl -lfftw3 -lm -lbcl 

OBJ= resize_copy.o zero_padding.o resize_filter.o

all: resize_copy zero_padding resize_filter

resize_copy : resize_copy.o
	$(CC) resize_copy.o $(LDFLAGS) -o resize_copy

resize_copy.o : resize_copy.c

zero_padding : zero_padding.o fft.o
	$(CC) zero_padding.o fft.o $(LDFLAGS) -o zero_padding

resize_zeropadding.o : zero_padding.c fft.h

fft.o : fft.c fft.h

resize_filter : resize_filter.o
	$(CC) resize_filter.o $(LDFLAGS) -o resize_filter

resize_filter.o : resize_filter.c

clean :
	rm -f $(OBJ) resize_copy zero_padding resize_filter
	rm -f *~


