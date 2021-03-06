/**
 * @file  mm_label.c
 * @brief count connected components in pnm image
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pnm.h>
#include <memory.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int _find(int p, int* roots)
{
    while(roots[p]!=p) p=roots[p];
    return p;
}

int _union(int r0, int r1, int* roots)
{
    if(r0 == r1) return r0;
    if(r0 == -1) return r1;
    if(r1 == -1) return r0;
    if(r0 <  r1){
        roots[r1] = r0;
        return r0;
    }else{
        roots[r0]=r1;
        return r1;
    }
}

int _add(int p, int r, int* roots)
{
    if(r==-1)
        roots[p]=p;
    else
        roots[p]=r;
    return roots[p];
}

void 
process(pnm ims, char *imd_name){
    int             w     = pnm_get_width(ims);
    int             h     = pnm_get_height(ims);
    int             sz    = w*h;
    unsigned short *ps    = pnm_get_channel(ims, NULL, PnmRed);
    int             p     = 0;
    int             r     = -1;
    int             l     = 0;
    int            *roots = malloc(w*h*sizeof(int));
    pnm             imd   = pnm_new(w,h, PnmRawPpm);
    unsigned short *res   = pnm_get_image(imd);
    unsigned short fond;

    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            r = -1;
            if( j>0 && (*(ps-1)==(*ps)) )
                r = _union(_find(p-1, roots), r, roots);
            if( (i>0 && j>0) && (*(ps-1-w)==(*ps)) )
                r = _union(_find(p-1-w, roots), r, roots);
            if( i>0 && (*(ps-w)==(*ps)) )
                r = _union(_find(p-w, roots), r, roots);
            if( (j<(w-1) && i>0) && (*(ps+1-w)==(*ps)) )
                r = _union(_find(p+1-w, roots), r, roots);
            r = _add(p, r, roots);
            p++;
            ps++;
        }
    }
    for(p=0; p<sz; p++){
        roots[p] = _find(p, roots);
    }
    for(p=0; p<sz; p++){
        if(roots[p]==p){
                roots[p] = l++;
        }
        else
            roots[p] = roots[roots[p]];
    }

    //Coloration des composantes
    for(p=0; p<sz; p++) {
        *res++ = roots[p]*225;
        *res++ = roots[p]*125;
        *res++ = roots[p]*75;
    }
    /**********CALCUL ANGLE**********/
    fond = roots[0];
    int coordX[l];
    int coordY[l];
    int n[l];
    for(int i = 0; i < l;i++){
        coordX[i] = 0;
        coordY[i] = 0;
        n[i] = 0;
    }
    for(int y = 0; y < h;y++)
        for(int x = 0; x < w;x++){
            int offset = x+y*w;
            if(roots[offset] != fond){
                coordX[roots[offset]] += x;
                coordY[roots[offset]] += y;
                n[roots[offset]]++;
            }
        }
    for(int i = 0; i < l; i++)
        if(n[i]){
            coordX[i] /= n[i];
            coordY[i] /= n[i];
        }

    float a = coordX[4] - coordX[2];
    float b = coordY[4] - coordY[2];
    float alpha = atan(b/a);
    float angle = -alpha*M_PI/180;

    printf("angle %f\n",angle);

    pnm_save(imd, PnmRawPpm, imd_name);

    fprintf(stderr, "mm_labels: %d\n", l);
    pnm_free(imd);
    free(roots);
}

void
usage(char* s)
{
    fprintf(stderr,"%s <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

#define PARAM 2
int
main(int argc, char* argv[])
{
    if(argc != PARAM+1)
        usage(argv[0]);
    pnm pnm_ims = pnm_load(argv[1]);
    process(pnm_ims, argv[2]);
    pnm_free(pnm_ims);

    return EXIT_SUCCESS;
}
