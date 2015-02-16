#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>
#include <math.h>

#define PARAM 3
#define DELTAT 0.25

void change(float* buffer1, float* buffer2, int size){
    for(int i=0; i<size; i++) {
        buffer1[i] = buffer2[i];
    }
}

void process(int n, char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name); //Image source
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int size = width*height;

    pnm imd = pnm_new(width,height,PnmRawPpm); //Image de sortie

    //Buffers pour calculer le laplacien
    float *buffer1 = malloc(size*sizeof(float));
    float *buffer2 = malloc(size*sizeof(float));
    float laplacien = 0.0;
    float x1,x2,x3,x4,x5;

    for(int i = 0; i < height;i++){
        for(int j = 0; j < width;j++){
            buffer1[j+height*i] = pnm_get_component(ims,i,j,0);
            buffer2[j+height*i] = pnm_get_component(ims,i,j,0);
        }
    }

    for(int k = 0; k < n;k++){
        for(int i = 0; i < height;i++)
            for(int j = 0; j < width;j++){
                int offset = j+height*i;
                if(j < width-1)
                    x1 = buffer1[j+1+height*i];
                else
                    x1 = buffer1[offset];
                if(j != 0)
                    x2 = buffer1[j-1+height*i];
                else
                    x2 = buffer1[offset];
                if(i < height-1)
                    x3 = buffer1[(i+1)*height+j];
                else
                    x3 = buffer1[offset];
                if(i!=0)
                    x4 = buffer1[(i-1)*height+j];
                else
                    x4 = buffer1[offset];
                x5 = buffer1[offset]*4;
                laplacien = x1 + x2 + x3 + x4 - x5;
                buffer2[offset] = buffer1[offset] + laplacien*DELTAT;
            }
        change(buffer1,buffer2,size);
    }

    for(int i = 0; i < height;i++)
        for(int j = 0; j < width;j++)
            for(int k  = 0; k < 3;k++){
                int offset = j+height*i;
                if(buffer2[offset] > 255)
                    buffer2[offset] = 255;
                if(buffer2[offset] < 0)
                    buffer2[offset] = 0;
                pnm_set_component(imd,i,j,k,(unsigned short)buffer2[offset]);
            }

    pnm_save(imd,PnmRawPpm,imd_name);
    pnm_free(ims);
    pnm_free(imd);
    free(buffer1);
    free(buffer2);
}

void usage(char *s){
    printf("Usage: %s <n> <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);
    process(atoi(argv[1]),argv[2],argv[3]);

    return EXIT_SUCCESS;
}
