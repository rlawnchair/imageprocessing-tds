#include <complex.h>
#include <fftw3.h>
#include <math.h>
#include "bcl.h"
#include "fft.h"

extern fftw_complex *fft_forward(int height, int width, unsigned short* gray_image){
    fftw_complex *frequency_repr = fftw_malloc(width*height*sizeof(fftw_complex));
    fftw_complex *spatial_repr = fftw_malloc(width*height*sizeof(fftw_complex));
    for(int i = 0; i < width*height; i++)
    {
        spatial_repr[i] = gray_image[i]+I*0;
        spatial_repr[i]=pow(-1,(i%width)+((i-(i%width))/width))*spatial_repr[i];
    }
    fftw_plan plan = fftw_plan_dft_2d(height, width, spatial_repr, frequency_repr, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_free(spatial_repr);
    fftw_destroy_plan(plan);
    return frequency_repr;
}

extern unsigned short *fft_backward(int height, int width, fftw_complex* freq_repr){
    unsigned short *channel = fftw_malloc(width*height*sizeof(unsigned short));
    fftw_complex *image_complex = fftw_malloc(width*height*sizeof(fftw_complex));;

    fftw_plan plan = fftw_plan_dft_2d(height, width, freq_repr, image_complex,FFTW_BACKWARD,FFTW_ESTIMATE);
    fftw_execute(plan);
    for(int i = 0 ; i < width*height; i++)
    {
        channel[i] = (unsigned short)(creal(image_complex[i])/(width*height));
        channel[i]=pow(-1,(i%width)+((i-(i%width))/width))*channel[i];
    }

    fftw_free(image_complex);
    fftw_destroy_plan(plan);
    return channel;
}

extern void fft_fr_to_spectra(int height, int width, fftw_complex* freq_repr, float* as, float* ps){
    int size = width*height;
    for(int i = 0; i < size; i++)
    {
        as[i]=hypot(creal(freq_repr[i]),cimag(freq_repr[i]));
        ps[i] = atan2(cimag(freq_repr[i]),creal(freq_repr[i]));
    }
}

extern void fft_spectra_to_fr(int height, int width, float* as, float* ps, fftw_complex* freq_repr){
    int size = width*height;
    for(int i=0; i<size; i++)
        freq_repr[i] = as[i] *  cos(ps[i]) + I * (as[i] * sin(ps[i]));
}
