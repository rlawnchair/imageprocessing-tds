#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pnm.h>
#include "bcl.h"

#define M_PI 3.14159265358979323846

int min(int a,int b){
    return a <= b ? a : b;
}

int max(int a,int b){
    return a >= b ? a : b;
}

float boite(float x) {
    if( x < 0.5 && x >= -0.5)
        return 1;
    return 0;
}

float tent(float x) {
    if(x >= -1 && x <= 1)
        return 1 - fabsf(x);
    else
        return 0;
}

float gaussian(float x) {
    return 1/sqrt(M_PI*2)*exp(-x*x/2);
}

float bell(float x) {
    if(fabsf(x) <= 0.5)
        return -x*x + 0.75;
    if(fabsf(x) > 0.5 && fabsf(x) <= 1.5)
        return 0.5*(fabsf(x)-1.5)*(fabsf(x)-1.5);
    return 0;
}

float MitchellNetravali(float x){
    if(x >= -1 && x<=1)
        return (float)7/6 * fabsf(x*x*x) -2*x*x + (float)8/9;

    if((x >= -2 && x <= -1) || (x >= 1 && x <= 2))
        return -(float)7/18 * fabsf(x*x*x) + 2*x*x - (float)10/3*fabsf(x) + (float)16/9;

    return 0;
}

void rotateClock(pnm imd)
{
    int h = pnm_get_height(imd);
    int w = pnm_get_width(imd);


    pnm imd2 = pnm_new(w ,h , PnmRawPpm);

    for(int i = 0; i < w;i++)
        for(int j = 0; j < h; j++)
            pnm_set_component(imd2,j,i,PnmRed,pnm_get_component(imd,i,j,PnmRed));

    unsigned short *buffer=(unsigned short*) malloc(sizeof(unsigned short)*h*w);
    buffer=pnm_get_channel(imd2,buffer,0);
    for(int i = 0; i < 3 ; i ++)
        pnm_set_channel(imd,buffer,i);


    pnm_free(imd2);

}

void resize_filter(int factor, char *ims_name, char *output, float (*f)(float x)){
    pnm ims = pnm_load(ims_name);

    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int width2 = width*factor;
    int height2 = height*factor;

    pnm output_ims = pnm_new(width2, height2,PnmRawPpm);
    float LF = factor/2;

    //traitement des lignes
    for(int i = 0; i < height2 ; i ++){
        for(int j = 0; j < width2; j++){
            float l = j / factor;
            int left = max(l - LF,0);
            int right = min(l+LF, width - 1);
            float S=0;

            for(int k=left; k <= right; k++){
                S += pnm_get_component(ims,i/factor,k,PnmRed) * f(k-l);

            }
            if(S > 255.0)
                S = 255.0;

            if(S < 0.0)
                S = 0.0;
            pnm_set_component(output_ims,i,j,0,S);
            pnm_set_component(output_ims,i,j,1,S);
            pnm_set_component(output_ims,i,j,2,S);
        }
    }

    rotateClock(ims);
    for(int i = 0; i < height2 ; i ++){
        for(int j = 0; j < width2; j++){

            float l = j/factor;
            int left = max(l - LF,0);
            int right = min(l+LF, width - 1);
            float S=0;

            for(int k=left; k <= right; k++){
                S += pnm_get_component(ims,i/factor,k,PnmRed) * f(k-l);

            }
            if(S > 255.0)
                S = 255.0;

            if(S < 0.0)
                S = 0.0;
            pnm_set_component(output_ims,i,j,0,S);
            pnm_set_component(output_ims,i,j,1,S);
            pnm_set_component(output_ims,i,j,2,S);
        }
    }
    rotateClock(output_ims);
    rotateClock(output_ims);
    rotateClock(output_ims);

    //    //traitement des colonnes
    //    for(int i = 0; i < width2; i ++){
    //        for(int j = 0; j < height2; j++){

    //            int l = j/factor;
    //            int left = max(l - LF,0);
    //            int right = min((int)round((double)l+LF), height2 - 1);

    //            unsigned short S=0;

    //            for(int k=left; k < right; k++){
    //                S += pnm_get_component(ims,k,l,PnmRed) * f(k-l);
    //            }
    //            pnm_set_component(output_ims,j,i,0,S);
    //            pnm_set_component(output_ims,j,i,1,S);
    //            pnm_set_component(output_ims,j,i,2,S);
    //        }
    //    }

    pnm_save(output_ims,PnmRawPpm,output);
    pnm_free(ims);
    pnm_free(output_ims);

}

int main(int argc, char* argv[]){
    if (argc != 5){
        printf("Il manque des arguments");
        exit(0);
    }
    int factor = atoi(argv[1]);
    char *filter = argv[2];
    char *ims_name = argv[3];
    char *output = argv[4];
    float (*func_filter)(float);

    if (strcmp(filter, "box") == 0)
        func_filter = boite;
    else if (strcmp(filter, "tent") == 0)
        func_filter = tent;
    else if (strcmp(filter, "gauss") == 0)
        func_filter = gaussian;
    else if (strcmp(filter, "bell") == 0)
        func_filter = bell;
    else if (strcmp(filter, "mitch") == 0)
        func_filter = MitchellNetravali;
    else
        exit(1);
    resize_filter(factor, ims_name, output, func_filter);

    return EXIT_SUCCESS;
}
