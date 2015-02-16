/**
 * @author Hitier Jérémy
 */
#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>

void Usage(char *s){
    printf("Usage : %s <ima> <imb> <imd>\n",s);
    exit(0);
}
int main(int argc, char* argv[]){
    if(argc != 4)
        Usage(argv[0]);

    char *ima_name = argv[1];
    char *imb_name = argv[2];
    char *imd_name = argv[3];

    pnm ima = pnm_load(ima_name);
    pnm imb = pnm_load(imb_name);
    int width = pnm_get_width(ima);
    int height = pnm_get_height(imb);

    pnm imd = pnm_new(width,height,PnmRawPpm);

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++){
            int addition = pnm_get_component(ima,i,j,0)+pnm_get_component(imb,i,j,0);
            if(addition > 255)
                addition = 255;
            for(int k = 0; k < 3; k++){
                pnm_set_component(imd,i,j,k,addition);
            }
        }
    pnm_save(imd,PnmRawPpm,imd_name);

    pnm_free(ima);
    pnm_free(imb);
    pnm_free(imd);
}
