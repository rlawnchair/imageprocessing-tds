#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>
#include <math.h>
#include <string.h>
#include "fft.h"

#define PARAM 8

void spectral_normalization(int width, int height, float *as, float *ps){
    double max_as = 0;
    double max_ps = 0;
    for(int i = 0; i < width*height+1;  i++){
        if(as[i] > max_as)
            max_as = as[i];
        if(ps[i] > max_ps)
            max_ps = ps[i];
    }
    for(int i = 0; i < width*height;  i++){
        as[i] /= max_as;
        ps[i] /= max_ps;
        as[i] = pow(as[i], 0.15) * pnm_maxval;
        ps[i] = pow(ps[i], 0.15) * pnm_maxval;
    }
}


float d(float u, float v){
    return sqrt(u*u+v*v);
}

float d1(int u, int v, int u0, int v0){
    return sqrt(pow(u-u0,2)+pow(v-v0,2));
}

float d2(int u, int v, int u0, int v0){
    return sqrt(pow(u+u0,2)+pow(v+v0,2));
}

float lp(float d0, float n, float u, float v){
    return 1/(1+pow(d(u,v)/d0,2*n));
}

float hp(float d0, float n, float u, float v){
    return 1/(1+pow(d0/d(u,v),2*n));
}

float br(float d0, float n, float u, float v, float w){
    float duv = d(u,v);
    return 1/(1+pow((duv*w)/(pow(duv,2)-pow(d0,2)),2*n));
}

float bp(float d0, float n, float u, float v, float w){
    return 1-br(d0,n,u,v,w);
}

float no(float d0, float n, float u, float v, float w, float u0, float v0){
    return 1/(1+pow(pow(d0,2)/(d1(u,v,u0,v0)*d2(u,v,u0,v0)),2*n));
}


void process(char *ims_name, char *imd_name, /*float (*f)(float,...)*/char *filter_name, float d0, float n, float w, float u0, float v0){
    printf("Process\n");
    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width, height, PnmRawPpm);
    pnm amp = pnm_new(width, height, PnmRawPpm);
    pnm phase = pnm_new(width, height, PnmRawPpm);
    unsigned short *res = malloc(width*height*sizeof(unsigned short));
    float *as = malloc(width*height*sizeof(float));
    float *ps = malloc(width*height*sizeof(float));

    res = pnm_get_channel(ims,res,0);
    fftw_complex *fft = fft_forward(height, width, res);
    fft_fr_to_spectra(height, width, fft, as, ps);

    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++){
            if(!strcmp(filter_name,"lp")){
                as[y*width+x] *= lp(d0, n, (x-width/2), (y-width/2));
            }
            else if(!strcmp(filter_name,"hp")){
                as[y*width+x] *= hp(d0, n, (x-width/2), (y-width/2));
            }
            else if(!strcmp(filter_name,"br")){
                as[y*width+x] *= br(d0, n, (x-width/2), (y-width/2), w);
            }
            else if(!strcmp(filter_name,"bp")){
                as[y*width+x] *= bp(d0, n, (x-width/2), (y-width/2), w);
            }
            else if(!strcmp(filter_name,"no")){
                as[y*width+x] *= no(d0, n, (x-width/2), (y-width/2), w, u0, v0);
            }
            else{
                exit(EXIT_FAILURE);
            }
            //as[y*width+x] *= (*f)(d0, n, (x-width/2), (y-width/2), w, u0, v0);
        }
    fftw_complex *tmp = fftw_malloc(width*height*sizeof(fftw_complex));
    fft_spectra_to_fr(height, width, as, ps, tmp);
    spectral_normalization(width,height,as,ps);

    free(res);
    res = fft_backward(height, width, tmp);

    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++){
            for(int k = 0; k < 3;k++){
                if(res[y*width+x] > 255)
                    res[y*width+x] = -res[y*width+x];
                pnm_set_component(amp,y,x,k,(unsigned short)as[y*width+x]);
                pnm_set_component(amp,y,x,k,(unsigned short)ps[y*width+x]);
                pnm_set_component(imd,y,x,k,(unsigned short)res[y*width+x]);
            }
        }

    pnm_save(imd, PnmRawPpm, imd_name);
    pnm_save(amp, PnmRawPpm, "amp.ppm");
    pnm_save(phase, PnmRawPpm, "phase.ppm");
    free(res);
    free(as);
    free(ps);
    fftw_free(fft);
    fftw_free(tmp);
    pnm_free(imd);
    pnm_free(ims);
    pnm_free(amp);
    pnm_free(phase);
}

void usage(char *s){
    printf("Usage : %s <ims> <imd> <filter> <d0> <n> <w> <u0> <v0>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);
    char *ims_name = argv[1];
    char *imd_name = argv[2];
    char *filter_name = argv[3];
    //float (*filter)(float,...);
    float d0 = atof(argv[4]);
    float n = atof(argv[5]);
    float w = atof(argv[6]);
    float u0 = atof(argv[7]);
    float v0 = atof(argv[8]);
    if(strcmp(filter_name,"lp") && strcmp(filter_name,"hp") && strcmp(filter_name,"br") && strcmp(filter_name,"bp") && strcmp(filter_name,"no"))
        exit(EXIT_FAILURE);
    /*
    if(!strcmp(filter_name,"lp")){
        filter = lp;
    }
    else if(!strcmp(filter_name,"hp")){
        filter = hp;
    }
    else if(!strcmp(filter_name,"br")){
        filter = br;
    }
    else if(!strcmp(filter_name,"bp")){
        filter = bp;
    }
    else if(!strcmp(filter_name,"no")){
        filter = no;
    }
    else{
        exit(EXIT_FAILURE);
    }
*/
    process(ims_name, imd_name, filter_name, d0, n, w, u0, v0);

    return EXIT_SUCCESS;
}
