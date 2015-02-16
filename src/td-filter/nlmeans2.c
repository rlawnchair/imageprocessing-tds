#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>
#include <math.h>

#define PARAM 3
#define NEIGH 11
#define PATCH 5

float dist(float p, float q){
    int res = 0.0;
    for(int i = 0; i < PATCH*PATCH;i++)
//        res +=
    return res/(PATCH*PATCH);
}

float gaussian(float sigma, float p, float q) {
    return exp(-dist(p,q)/(2*sigma*sigma));
}

void process(int sigma, char *ims_name, char *imd_name){

    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width, height, PnmRawPpm);

    float res1 = 0.0;
    float res2 = 0.0;
    float tmp = 0.0;

    for(int y = 0; y<height; y++)
        for(int x = 0; x<width; x++){
            for(int j=-sigma; j<sigma; j++)
                for(int i=-sigma; i<sigma; i++){
                    if(i+x<width  && x+i>0 && j+y<height && y+j>0) {
                        tmp = gaussian(sigma,pnm_get_component(ims,y,x,0),pnm_get_component(ims,y+j,x+i,0));
                        res1 += tmp*(pnm_get_component(ims,y+j,x+i,0));
                        res2 += tmp;
                    }
                }
            for(int k=0; k<3; k++)
                pnm_set_component(imd,y,x,k,res1/res2);
            res1 = 0.0;
            res2 = 0.0;
        }

    pnm_save(imd, PnmRawPpm, imd_name);
    pnm_free(imd);
    pnm_free(ims);
}

void usage(char *s){
    printf("Usage : %s <sigma> <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);
    int sigma = atoi(argv[1]);
    char *ims_name = argv[2];
    char *imd_name = argv[3];

    process(sigma, ims_name, imd_name);

    return EXIT_SUCCESS;
}
