#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>

#define PARAM 3

void process(char *ims_name, char *imd_name, float angle){
    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width, height, PnmRawPpm);


    pnm_save(imd, PnmRawPpm, imd_name);
    pnm_free(ims);
    pnm_free(imd);
}

void usage(char *s){
    printf("Usage : %s <ims> <imd> <rotation>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);

    char *ims_name = argv[1];
    char *imd_name = argv[2];
    float angle = atof(argv[3]);

    process(ims_name, imd_name, angle);

    return EXIT_SUCCESS;
}
