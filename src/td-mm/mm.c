/**
 * @author Hitier Jérémy
 */
#include "mm.h"
#include <math.h>

int MIN(int a , int b){
    return a>=b?a:b;
}

int MAX(int a , int b){
    return a<=b?a:b;
}

void vec_se(pnm output, int i, int j, int channel, int halfsize) {
    int a = i+halfsize, b = halfsize-i, c = j+halfsize, d = halfsize-j;
    pnm_set_component(output,a,c,channel,255);
    pnm_set_component(output,b,c,channel,255);
    pnm_set_component(output,b,d,channel,255);
    pnm_set_component(output,a,d,channel,255);
}

void sqare(pnm output,int size){
    for(int i = 0 ; i < size; i++)
        for(int j =0 ; j < size;j++){
            pnm_set_component(output,i,j,PnmRed,255);
            pnm_set_component(output,i,j,PnmGreen,255);
            pnm_set_component(output,i,j,PnmBlue,255);
        }
}

void diamond(pnm output, int size){
    for(int i=0; i<size+1; i++)
        for(int j=0; j<size+1; j++)
            if(i+j<= size)
                for(int channel = 0; channel < 3; channel++)
                    vec_se(output, i, j, channel, size);
}

void disk(pnm output, int size){
    for(int i=0; i<size+1; i++)
        for(int j=0; j<size+1; j++)
            if(hypot(j,i) <= size)
                for(int channel = 0; channel < 3; channel++)
                    vec_se(output, i, j, channel, size);
}

pnm mm_se(int shape, int halfsize){
    int size = 2*halfsize+1;
    pnm output = pnm_new(size,size,PnmRawPpm);
    switch(shape){
    case 0:
        sqare(output,size);
        break;
    case 1:
        diamond(output,halfsize);
        break;
    case 2:
        disk(output,halfsize);
        break;
    default:
        printf("Bad shape value\n");
        break;
    }
    return output;
}

void mm_process(int shape, int halfsize, pnm ims, pnm imd, void (*pf)(unsigned short, unsigned short *)){
    pnm Shape = mm_se(shape,halfsize);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int size = 2*halfsize+1;
    unsigned short tmp;
    int x,y;

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            tmp = pnm_get_component(ims,i,j,0);
            for(int k = 0; k < size; k++)
                for(int l = 0; l < size; l++)
                    if(pnm_get_component(Shape,k,l,0)) { //On ne prend que la partie "blanche" de l'élément structurant
                        x = j - halfsize + l;
                        y = i - halfsize + k;
                        if(x < width && y < height && x > 0 && y > 0)
                            pf(pnm_get_component(ims,y,x,0),&tmp);
                    }
            for(int channel=0; channel<3; channel++)
                pnm_set_component(imd,i,j,channel,tmp);
        }
    pnm_free(Shape);
}

void mm_lesser(unsigned short val, unsigned short *min){
    *min = MIN(*min, val);
}

void mm_greater(unsigned short val, unsigned short *max){
    *max = MAX(*max, val);
}
/*
void usage(){
    printf("Usage : ");
    exit(0);
}

int main(int argc, char*argv[]){
    pnm res = mm_se(1,200);
    pnm_save(res,PnmRawPpm,"output.ppm");
    pnm_free(res);
    return EXIT_SUCCESS;
}
*/
