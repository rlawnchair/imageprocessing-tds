/**
 * @author Hitier Jérémy
 */
#include "mm.h"

void Usage(char *s){
    printf("Usage : %s <se> <hs> <ims> <imd>\n",s);
    exit(0);
}
int main(int argc, char* argv[]){
    if(argc != 5)
        Usage(argv[0]);

    int se = atoi(argv[1]);
    int hs = atoi(argv[2]);
    char *ims_name = argv[3];
    char *imd_name = argv[4];

    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    pnm imd = pnm_new(width,height,PnmRawPpm);
    pnm res = pnm_new(width,height,PnmRawPpm);

    mm_process(se,hs,ims,imd,mm_lesser);
    mm_process(se,hs,imd,res,mm_greater);

    pnm_save(res,PnmRawPpm, imd_name);

    pnm_free(ims);
    pnm_free(imd);
    pnm_free(res);
}
