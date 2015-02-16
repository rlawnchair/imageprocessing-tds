#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pnm.h>

#define PARAM 4


float gaussian(float sigma, float k) {
    return exp(-k*k/(2*sigma*sigma));
}

void process(int sigma_s, int sigma_g, char *ims_name, char *imd_name) {
    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width, height, PnmRawPpm);

    float res1 = 0.0;
    float res2 = 0.0;
    float tmp = 0.0;

    for(int y = 0; y<height; y++)
        for(int x = 0; x<width; x++){
            for(int j=-sigma_s; j<sigma_s; j++)
                for(int i=-sigma_s; i<sigma_s; i++){
                    if(i+x<width  && x+i>0 && j+y<height && y+j>0) {
                        tmp = gaussian(sigma_s,hypotf(i,j))*gaussian(sigma_g,abs(pnm_get_component(ims,y,x,0)-pnm_get_component(ims,y+j,x+i,0)));
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

void usage (char *s){
    fprintf(stderr, "Usage: %s <sigma_s> <sigma_g> <ims> <imd>\n", s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if (argc != PARAM+1)
        usage(argv[0]);

    float sigma_s  = atoi(argv[1]);
    float sigma_g  = atoi(argv[2]);
    char* ims_name = argv[3];
    char* imd_name = argv[4];

    if( sigma_s <= 0 && sigma_g <= 0) {
        fprintf(stderr, "sigma > 0\n");
        exit(EXIT_FAILURE);
    }
    process(sigma_s,sigma_g,ims_name,imd_name);
    return EXIT_SUCCESS;
}
