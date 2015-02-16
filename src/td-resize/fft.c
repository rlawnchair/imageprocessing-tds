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
        spatial_repr[i] = gray_image[i];
        if(((i%width)+(i-(i%width))/width)%2 != 0)
            spatial_repr[i] = -1*spatial_repr[i];
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
        if(((i%width)+(i-(i%width))/width)%2 != 0)
            image_complex[i] = -1*image_complex[i];
        float tmpChan = creal(image_complex[i])/(width*height);
        if(tmpChan < 0)
            channel[i] = 0;
        else if(tmpChan > 255)
            channel[i] = 255;
        else
            channel[i] = tmpChan;
    }

    fftw_free(image_complex);
    fftw_destroy_plan(plan);
    return channel;
}
