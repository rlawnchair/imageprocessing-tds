#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pnm.h>
#include <math.h>

#define PARAM 5
#define DELTAT 0.25

typedef struct grad grad;
struct grad{
    float d1;
    float d2;
};

void change(float* buffer1, float* buffer2, int size){
    for(int i=0; i<size; i++) {
        buffer1[i] = buffer2[i];
    }
}

float c0(float s, float lambda) {
    s += 0;
    lambda  += 0;
    return 1;
}

float c1(float s, float lambda) {
    float slambda = s/lambda;
    return 1/(1 + (slambda)*(slambda));
}

float c2(float s, float lambda) {
    float slambda = s/lambda;
    return exp(-(slambda)*(slambda));
}

void process(int n, int lambda, float (*f)(float, float), char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name); //Image source
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int size = width*height;

    pnm imd = pnm_new(width,height,PnmRawPpm); //Image de sortie

    //Buffers pour calculer le laplacien
    float *buffer1 = malloc(size*sizeof(float));
    float *buffer2 = malloc(size*sizeof(float));
    float norme = 0.0;
    float x1,x2,x3,x4,x5;
    grad gradient;
    float detect = 0.0;
    float div = 0.0;
    float p1, p2;

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

                x5 = buffer1[offset];
                gradient.d1 = (x1 - x5);
                gradient.d2 = (x3 - x5);
                p1 = (x5 - x2);
                p2 = (x5 - x4);
                norme = sqrt(gradient.d1*gradient.d1+gradient.d2*gradient.d2);
                detect = f(norme,lambda);
                div = detect*(gradient.d1-p1) + detect*(gradient.d2-p2);
                buffer2[offset] = buffer1[offset] + div*DELTAT;
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
    printf("Usage: %s <n> <lambda> <function> <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);

    int   n        = atoi(argv[1]);
    float lambda   = atof(argv[2]);
    char* f_name   = argv[3];
    char* ims_name = argv[4];
    char* imd_name = argv[5];
    float  (*function)(float,float);
    if(n <= 0) {
        fprintf(stderr, "n > 0\n");
        exit(EXIT_FAILURE);
    }
    if(!strcmp(f_name,"c0"))
        function = c0;
    else if(!strcmp(f_name,"c1"))
        function = c1;
    else if(!strcmp(f_name,"c2"))
        function = c2;
    else
        exit(EXIT_FAILURE);

    process(n, lambda, function, ims_name, imd_name);

    return EXIT_SUCCESS;
}
