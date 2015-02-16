ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lfftw3 -lm -lbcl 

OBJ= heat_equation anisotropic_diffusion bilateral nlmeans butterworth

all: heat_equation anisotropic_diffusion bilateral nlmeans butterworth

heat_equation.o : heat_equation.c

heat_equation : heat_equation.o
	$(CC) heat_equation.o $(LDFLAGS) -o heat_equation

anisotropic_diffusion.o : anisotropic_diffusion.c

anisotropic_diffusion : anisotropic_diffusion.o
	$(CC) anisotropic_diffusion.o $(LDFLAGS) -o anisotropic_diffusion

bilateral.o : bilateral.c

bilateral : bilateral.o
	$(CC) bilateral.o $(LDFLAGS) -o bilateral

nlmeans.o : nlmeans.c

nlmeans : nlmeans.o
	$(CC) nlmeans.o $(LDFLAGS) -o nlmeans

fft.o : fft.c fft.h

butterworth.o : butterworth.c fft.c fft.h

butterworth : butterworth.o fft.o
	$(CC) butterworth.o fft.o $(LDFLAGS) -o butterworth

clean :
	rm -f $(OBJ) heat_equation.o anisotropic_diffusion.o bilateral.o nlmeans.o fft.o butterworth.o
	rm -f *~


