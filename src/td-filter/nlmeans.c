#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>
#include <math.h>

#define PARAM 3
#define R 5
#define N 2
#define DIV (2 * N + 1) * (2 * N + 1)

void process(int sigma, char *ims_name, char *imd_name){

    pnm ims = pnm_load(ims_name);
    int width = pnm_get_width(ims);
    int height = pnm_get_height(ims);
    int size = width * height;
    pnm imd = pnm_new(width, height, PnmRawPpm);

    unsigned short *buffer_ims = malloc(3*size*sizeof(unsigned short));
    unsigned short *res = malloc(3*size*sizeof(unsigned short));

    buffer_ims = pnm_get_image(ims);
    res = pnm_get_image(imd);

    float *current = malloc(3*size*sizeof(float));
    float *next = malloc(3*size* sizeof(float));

    for(int i = 0; i < size; i++) {
        current[i] = (float)buffer_ims[i*3];
    }

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int offset = y*width+x;
            float numerator = 0.0;
            float denominator = 0.0;

            for(int y0 = y - R; y0 < y + R + 1; y0++) {
                for(int x0 = x - R; x0 < x + R + 1; x0++) {
                    if(y0 >= 0 && y0 < height && x0 >= 0 && x0 < width) {
                        const int offset0 = y0*width+x0;
                        float dist = 0.0;
                        for(int patch_y = -N; patch_y < N + 1; patch_y++) {
                            for(int patch_x = -N; patch_x < N + 1; patch_x++) {
                                if(y + patch_y >= 0 && y + patch_y < height && x + patch_x >= 0 && x + patch_x < width && y0 + patch_y >= 0 && y0 + patch_y < height && x0 + patch_x >= 0 && x0 + patch_x < width) {
                                    int offset_patch = patch_y * width + patch_x;
                                    float value_p = current[offset + offset_patch];
                                    float value_q = current[offset0 + offset_patch];
                                    dist += abs(value_p - value_q);
                                }
                            }
                        }
                        float weight = exp(-dist/(2*sigma*sigma));
                        numerator += weight*current[offset0];
                        denominator += weight;
                    }
                }
            }
            next[offset] = numerator/denominator;
        }
    }

    for(int i = 0; i < size; i++) {
        res[i*3] = next[i];
        res[i*3+1] = next[i];
        res[i*3+2] = next[i];
    }


    pnm_save(imd, PnmRawPpm, imd_name);
    free(current);
    free(next);
    pnm_free(imd);
    pnm_free(ims);
}

void usage(char *s){
    printf("Usage : %s <sigma> <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    if(argc != PARAM+1)
        usage(argv[0]);
    int sigma = atoi(argv[1]);
    char *ims_name = argv[2];
    char *imd_name = argv[3];

    process(sigma, ims_name, imd_name);

    return EXIT_SUCCESS;
}
