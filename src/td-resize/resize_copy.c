#include <stdio.h>
#include <stdlib.h>
#include "bcl.h"
#include <pnm.h>

void resize_copy(int factor, char *ims_name, char *output){
    pnm ims = pnm_load(ims_name);

    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm output_ims = pnm_new(width*factor,height*factor,PnmRawPpm);

    for(int i=0; i<height*factor; i++){
        for(int j=0; j<width*factor; j++){
            pnm_set_component(output_ims,i,j,PnmRed,pnm_get_component(ims,i/factor,j/factor,PnmRed));
            pnm_set_component(output_ims,i,j,PnmGreen,pnm_get_component(ims,i/factor,j/factor,PnmGreen));
            pnm_set_component(output_ims,i,j,PnmBlue,pnm_get_component(ims,i/factor,j/factor,PnmBlue));
        }
    }
    pnm_save(output_ims,PnmRawPpm,output);
    pnm_free(ims);
    pnm_free(output_ims);
}

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Il manque des arguments");
        exit(0);
    }
    int factor = atoi(argv[1]);
    char *ims_name = argv[2];
    char *output = argv[3];
    resize_copy(factor,ims_name, output);
    return EXIT_SUCCESS;
}
