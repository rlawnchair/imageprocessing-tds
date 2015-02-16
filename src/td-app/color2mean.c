/**
 * @author Hitier Jérémy
 * @brief Color2Mean
 */
#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>
#include <math.h>

#define PARAM 2

void process(char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width,height,PnmRawPpm);
    float moyenne = 0.0;
    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++){
            moyenne = (pnm_get_component(ims,y,x,0)+pnm_get_component(ims,y,x,1)+pnm_get_component(ims,y,x,2))/3;
            for(int k = 0; k < 3;k++)
                pnm_set_component(imd,y,x,k,(unsigned short)moyenne);
        }
    pnm_save(imd,PnmRawPpm,imd_name);
    pnm_free(ims);
    pnm_free(imd);
}

void usage(char *s){
    printf("Usage : %s <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);

    process(argv[1],argv[2]);

    return EXIT_SUCCESS;
}
