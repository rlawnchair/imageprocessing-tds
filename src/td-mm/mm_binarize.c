/**
 * @author Hitier Jérémy
 */
#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>

void Usage(char *s){
    printf("Usage : %s <min> <max> <ims> <imd>\n",s);
    exit(0);
}

void binarize(int min, int max, char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width, height, PnmRawPpm);

    for(int i = 0 ; i < height; i++){
        for(int j = 0; j < width; j++){
            int tmpComp = pnm_get_component(ims,i,j,0);
            if(tmpComp >= min &&  tmpComp <= max){
                pnm_set_component(imd,i,j,0,255);
                pnm_set_component(imd,i,j,1,255);
                pnm_set_component(imd,i,j,2,255);
            }
            else{
                pnm_set_component(imd,i,j,0,0);
                pnm_set_component(imd,i,j,1,0);
                pnm_set_component(imd,i,j,2,0);
            }
        }
    }

    pnm_save(imd,PnmRawPpm,imd_name);

    pnm_free(ims);
    pnm_free(imd);
}

int main(int argc, char* argv[]){
    if(argc!=5)
        Usage(argv[0]);
    int min = atoi(argv[1]);
    int max = atoi(argv[2]);
    char *ims_name = argv[3];
    char *imd_name = argv[4];

    binarize(min,max,ims_name,imd_name);
}
