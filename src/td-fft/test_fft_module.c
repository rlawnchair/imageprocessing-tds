
#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <fftw3.h>

#include "fft.h"
#include <pnm.h>

/**
 * @file test_fft_module.c
 * @brief test the behaviors of functions in fft module
 */


/**
 * @brief test the forward and backward functions
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_for_backward(pnm ims, char* name)
{
  
  int h=pnm_get_height(ims);
  int w=pnm_get_width(ims);
  unsigned short *buffer=(unsigned short*) malloc(sizeof(unsigned short)*h*w);
  fftw_complex *tf=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*h*w);
  pnm res=pnm_new(w,h,PnmRawPpm);
  
  buffer=pnm_get_channel(ims,buffer,0);
  tf=fft_forward(w,h,buffer);
  buffer=fft_backward(w,h,tf);
  
  pnm_set_channel(res,buffer,0);
  pnm_set_channel(res,buffer,1);
  pnm_set_channel(res,buffer,2);
 
  pnm_save(res,PnmRawPpm,"FB_lena.ppm");

   free(buffer);
  fftw_free(tf);
  pnm_free(res);  
}

/**
 * @brief test the creation of amplitude and phase spectrum
 *        and construct their images in ppm files
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_spectrum(pnm ims, char* name)
{
  
  int h=pnm_get_height(ims);
  int w=pnm_get_width(ims);
  unsigned short *buffer=(unsigned short*) malloc(sizeof(unsigned short)*h*w);
  fftw_complex *tf=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*h*w);
  float* as=(float*) malloc(sizeof(float)*h*w);
  float* ps=(float*) malloc(sizeof(float)*h*w);

  pnm res_as=pnm_new(w,h,PnmRawPpm);
  pnm res_ps=pnm_new(w,h,PnmRawPpm);
  pnm res=pnm_new(w,h,PnmRawPpm);

  buffer=pnm_get_channel(ims,buffer,0);
  tf=fft_forward(w,h,buffer);
    
  fft_fr_to_spectra(w,h,tf,as,ps);
  
  for(int i=0;i<h*w;i++)
    {
    
	pnm_set_component(res_as,i%w,((i-(i%w))/w),0,as[i]);
	pnm_set_component(res_as,i%w,((i-(i%w))/w),1,as[i]);
	pnm_set_component(res_as,i%w,((i-(i%w))/w),2,as[i]);

	pnm_set_component(res_ps,i%w,((i-(i%w))/w),0,ps[i]);
	pnm_set_component(res_ps,i%w,((i-(i%w))/w),1,ps[i]);
	pnm_set_component(res_ps,i%w,((i-(i%w))/w),2,ps[i]);

    }



 

  pnm_save(res_as,PnmRawPpm,"AS_lena.ppm");
  pnm_save(res_ps,PnmRawPpm,"PS_lena.ppm");

  fft_spectra_to_fr(w,h,as,ps,tf);

  buffer=fft_backward(w,h,tf);
  
  pnm_set_channel(res,buffer,0);
  pnm_set_channel(res,buffer,1);
  pnm_set_channel(res,buffer,2);
 
  pnm_save(res,PnmRawPpm,"FB_ASPS_lena.ppm");
  
  free(buffer);
  fftw_free(tf);
  pnm_free(res_as);
  pnm_free(res_ps);
}

/**
 * @brief test the modification of amplitude and phase spectrum and 
 *        construct output images
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_modification(pnm ims, char* name)
{
  
}


static void 
run(pnm ims, char* name)
{
  test_for_backward(ims, name);
  
  //test_for_backward(ims, name);
  test_spectrum(ims, name);
  test_modification(ims, name);
}


void 
usage (char *s)
{
  fprintf(stderr, "Usage: %s <input_name> \n", s);
  exit(EXIT_FAILURE);
}

int 
main(int argc, char *argv[])
{
  if (argc != 2)
    usage(argv[0]);
  pnm ims = pnm_load(argv[1]);
  run(ims, argv[1]);
  return EXIT_SUCCESS;
}
