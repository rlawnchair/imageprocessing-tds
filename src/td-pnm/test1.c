/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???*
 */

#include <stdlib.h>
#include <stdio.h>

#include <pnm.h>

void  
process(int width, int height, char* imd_name){
  
  pnm imd = pnm_new(width, height, PnmRawPpm);

  unsigned short *p = pnm_get_image(imd);
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      for(int z=0; z<3; z++){
	*p++ = pnm_maxval;
      }
    }    
  }
  int x0 = width/4;
  int x1 = 3*x0;
  int y0 = height/4;
  int y1 = 3*y0;
  unsigned short* image = pnm_get_image(imd);
  for(int x=x0; x<x1; x++){
    for(int y=y0; y<y1; y++){
      p  = image + pnm_offset(imd, y, x);
      *p = pnm_maxval - *p; 
      p++;
    }
  }
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <w> <h> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int 
main(int argc, char *argv[]){
  if (argc != param+1) usage(argv[0]);  
  int w = atoi(argv[1]);
  int h = atoi(argv[2]);
  char *imd_name = argv[3];
  process(w, h, imd_name);
  return EXIT_SUCCESS;
}
