#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pnm.h>
#include "fft.h"

void resize_zeropadding(int factor, char *ims_name, char *output){
    pnm ims = pnm_load(ims_name);

    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int width2 = width*factor;
    int height2 = height*factor;

    pnm output_ims = pnm_new(width2,height2,PnmRawPpm);

    unsigned short *channel = (unsigned short*)fftw_malloc(width*height*sizeof(unsigned short));
    unsigned short *res;
    fftw_complex *base;
    fftw_complex *newImg = fftw_malloc(width2*height2*sizeof(fftw_complex));

    memset(newImg, 0, width2*height2);
    //for(int i = 0; i < width2*height2; i++)
      //  newImg[i] = 0;

    channel = pnm_get_channel(ims,channel,PnmRed);
    base = fft_forward(height,width,channel);

    for(int i=(width2 / 2 - width / 2); i < (width2 / 2 + width / 2); i++){
        for(int j = (height2 / 2 - height / 2); j < (height2 / 2 + height / 2); j++){
            newImg[j+i*width2] = base[(i - (width2 / 2 - width / 2)) * width + (j - (width2 / 2 - width / 2))]*factor*factor;
        }
    }

    res = fft_backward(height2,width2,newImg);

    for(int i = 0; i < 3;i++)
        pnm_set_channel(output_ims,res,i);

    pnm_save(output_ims,PnmRawPpm,output);
    pnm_free(ims);
    pnm_free(output_ims);
    fftw_free(channel);
    fftw_free(res);
    fftw_free(base);
    fftw_free(newImg);
}

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Il manque des arguments");
        exit(0);
    }
    int factor = atoi(argv[1]);
    char *ims_name = argv[2];
    char *output = argv[3];
    resize_zeropadding(factor,ims_name, output);
    return EXIT_SUCCESS;
}

