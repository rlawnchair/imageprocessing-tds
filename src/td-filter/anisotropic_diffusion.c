#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pnm.h>
#include <math.h>

#define PARAM 5
#define DELTAT 0.25


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

float Div(int i, int j, int w, float* p1, float *p2){
    float res=p1[i+j*w];
    if(i!=0){
        res-=p1[i-1+j*w];
    }
    else{
        res-=p1[i+1+j*w];
    }
    res+=p2[i+j*w];
    if(j!=0){
        res-=p2[i+(j-1)*w];
    }
    else{
        res-=p1[i+(j+1)*w];
    }
    return res;
}

void process(int n, int lambda, float (*f)(float, float), char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name); //Image source
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int size = width*height;
    pnm imd = pnm_new(width,height,PnmRawPpm); //Image de sortie

    unsigned short *res = malloc(size*sizeof(unsigned short));
    float *res_copy = malloc(size*sizeof(float));
    float *tmp = malloc(size*sizeof(float));
    float *buffer1 = malloc(size*sizeof(float));
    float *buffer2 = malloc(size*sizeof(float));
    float *buffer_func = malloc(size*sizeof(float));
    float grad1, grad2;
    float norme = 0.0;

    res = pnm_get_channel(ims, res, 0);
    for(int i = 0; i < size;i++)
        res_copy[i] = (float)res[i];

    for(int k = 0; k < n; k++){
        for(int i = 0; i < size; i++){
            tmp[i] = res_copy[i];
        }
        for(int x = 0; x < width; x++)
            for(int y = 0; y < height; y++){
                int offset = x+y*width;
                if(x != width - 1)
                    grad1 = tmp[x+1+y*width] - tmp[offset];
                else
                    grad1 = tmp[x-1+y*width] - tmp[offset];
                if(y != height - 1)
                    grad2 = tmp[x+(y+1)*width] - tmp[offset];
                else
                    grad2 = tmp[x+(y-1)*width] - tmp[offset];
                norme = sqrt(grad1*grad1 + grad2*grad2);
                buffer1[offset]=grad1;
                buffer2[offset]=grad2;
                buffer_func[offset] = f(norme,lambda);
            }
        for(int x = 0; x < width;x++){
            for(int y = 0; y < height; y++){
                int offset = x+y*width;
                buffer1[offset]*=buffer_func[offset];
                buffer2[offset]*=buffer_func[offset];
                res_copy[offset]=tmp[offset]+DELTAT*Div(x,y,width,buffer1,buffer2);
            }
        }
    }
    for(int i = 0; i < size; i++)
        res[i] = (unsigned short)res_copy[i];

    for(int k = 0; k < 3; k++)
        pnm_set_channel(imd,res,k);

    pnm_save(imd,PnmRawPpm,imd_name);
    free(res);
    free(tmp);
    free(buffer1);
    free(buffer2);
    free(buffer_func);
    pnm_free(ims);
    pnm_free(imd);
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
