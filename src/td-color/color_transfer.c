/**
 * @file color_transfert
 * @brief transfert color from reference image to source image.
 *        Method from Reinhard et al. :
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley,
 *        'Color Transfer between Images', IEEE CGA special issue on
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pnm.h>

#define D 3
#define PARAM 3

typedef struct rgb rgb;
struct rgb{
    float r;
    float g;
    float b;
};

typedef struct lms lms;
struct lms{
    float l;
    float m;
    float s;
};

typedef struct lab lab;
struct lab{
    float l;
    float a;
    float b;
};

static float RGB2LMS[D][D] = {
    {0.3811, 0.5783, 0.0402},
    {0.1967, 0.7244, 0.0782},
    {0.0241, 0.1288, 0.8444}
};

static float LMS2LAB[D][D] = {
    {0.5773,  0.5773,  0.5773},
    {0.4082,  0.4082, -0.8164},
    {0.7071, -0.7071,  0}
};

static float LAB2LMS[D][D] = {
    {0.5773,  0.4082,  0.7071},
    {0.5773,  0.4082, -0.7071},
    {0.5773,  -0.8164,  0}
};

static float LMS2RGB[D][D] = {
    {4.4679,  -3.5873, 0.1193},
    {-1.2186, 2.3809,  -0.1624},
    {0.0497,  -0.2439, 1.2045}
};

// PROTOTYPES
void convert(float mat[D][D], float x, float y, float z, float *resX, float *resY, float *resZ);
void rgb2lms(rgb *colorRGB, lms *colorLMS, int size);
void lms2lAB();
void process(char *ims_name, char *imt_name, char *output_name);
void usage (char *s);

void convert(float mat[D][D], float x, float y, float z, float *resX, float *resY, float *resZ){
    *resX = mat[0][0]*x + mat[0][1]*x + mat[0][2]*x;
    *resY = mat[1][0]*y + mat[1][1]*y + mat[1][2]*y;
    *resZ = mat[2][0]*z + mat[2][1]*z + mat[2][2]*z;
}

void rgb2lms(rgb *colorRGB, lms *colorLMS, int size){
    for(int i = 0; i < size; i++){
        convert(RGB2LMS,colorRGB[i].r,colorRGB[i].g,colorRGB[i].b,&colorLMS[i].l,&colorLMS[i].m,&colorLMS[i].s);
    }
}

void lms2lab(lms *colorLMS, lab *colorLAB, int size){
    for(int i = 0; i < size; i++){
        if(colorLMS[i].l <= 0)
            colorLMS[i].l = 1;
        if(colorLMS[i].m <= 0)
            colorLMS[i].m = 1;
        if(colorLMS[i].s <= 0)
            colorLMS[i].s = 1;
        convert(LMS2LAB,log10(colorLMS[i].l),log10(colorLMS[i].m),log10(colorLMS[i].s),&colorLAB[i].l,&colorLAB[i].a,&colorLAB[i].b);
    }
}

void lab2lms(lab *colorLAB, lms *colorLMS, int size){
    for(int i = 0; i < size; i++){
        convert(LAB2LMS,colorLAB[i].l,colorLAB[i].a,colorLAB[i].b,&colorLMS[i].l,&colorLMS[i].m,&colorLMS[i].s);
        colorLMS[i].l = pow(10,colorLMS[i].l);
        colorLMS[i].m = pow(10,colorLMS[i].m);
        colorLMS[i].s = pow(10,colorLMS[i].s);
    }
}

void lms2rgb(lms *colorLMS, rgb *colorRGB, int size){
    for(int i = 0; i < size; i++){
        convert(LMS2RGB,colorLMS[i].l,colorLMS[i].m,colorLMS[i].s,&colorRGB[i].r,&colorRGB[i].g,&colorRGB[i].b);
    }
}

void process(char *ims_name, char *imt_name, char *output_name){

    // SOURCE IMAGE
    pnm ims = pnm_load(ims_name);
    int widthIms = pnm_get_width(ims);
    int heightIms = pnm_get_height(ims);
    int size = widthIms*heightIms;

    // TARGET IMAGE
    pnm imt = pnm_load(imt_name);
    int widthImt = pnm_get_width(imt);
    int heightImt = pnm_get_height(imt);

    // OUTPUT
    pnm output = pnm_new(widthIms, heightIms, PnmRawPpm);

    // RGBCOLOR
    rgb *colorRGB = malloc(size*sizeof(rgb));
    for(int i = 0; i < heightIms; i++)
        for(int j = 0; j < widthIms; j++){
            int offset = j+i*widthIms;
            colorRGB[offset].r = pnm_get_component(ims,i,j,0);
            colorRGB[offset].g = pnm_get_component(ims,i,j,1);
            colorRGB[offset].b = pnm_get_component(ims,i,j,2);
        }

    lms *colorLMS = malloc(size*sizeof(lms));
    lab *colorLAB = malloc(size*sizeof(lab));


    rgb2lms(colorRGB,colorLMS,size);
    //lms2lab(colorLMS,colorLAB,size);
    //lab2lms(colorLAB,colorLMS,size);
    lms2rgb(colorLMS,colorRGB,size);

    for(int i = 0; i < heightIms; i++)
        for(int j = 0; j < widthIms; j++){
            int offset = j+i*widthIms;
            pnm_set_component(output,i,j,0,(unsigned short)colorRGB[offset].r);
            pnm_set_component(output,i,j,1,(unsigned short)colorRGB[offset].g);
            pnm_set_component(output,i,j,2,(unsigned short)colorRGB[offset].b);
        }

    //for(int i = 0; i < size; i++)
    //  printf("%d | %d | %d\n",colorLMS[i].l,colorLMS[i].m,colorLMS[i].s);

    pnm_save(output,PnmRawPpm,output_name);
    pnm_free(ims);
    pnm_free(imt);
    pnm_free(output);
    free(colorRGB);
    free(colorLMS);
    free(colorLAB);
}

/**
 * @brief usage function :
 *        <ims>: the source image as reference
 *        <imt>: the target image to modify
 *        <imd>: the result
 */
void usage (char *s){
    fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);
    process(argv[1],argv[2],argv[3]);
    return EXIT_SUCCESS;
}
