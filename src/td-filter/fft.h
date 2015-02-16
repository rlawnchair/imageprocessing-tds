/**
 * @file fft.h
 * @brief header for fft module that implements functions for Fourier direct
 *        and inverse transforms and, amplitude and phase spectrum        
 */

#ifndef FFT_H
#define FFT_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <math.h>
#include "bcl.h"

/**
 * @brief perform a discrete 2D Fourier transform from grayscale image
 * @param int height : the input imag height
 * @param int width : the input image width
 * @param unsigned short* gray_image : the grayscale input image
 * @return the Fourrier transform in frequency domain
 */
extern fftw_complex 
*fft_forward(int height, int width, unsigned short* gray_image);

/**
 * @brief perform a discrete inverse 2D Fourier transform from freqency domain
 * @param int height : the input frequency domain height
 * @param int width : the input frequency domain width
 * @param fft_complex* freq_repr : the frequency input 
 * @return the obtain grayscale image from inverse Fourrier transform
 */
extern unsigned short 
*fft_backward(int height, int width, fftw_complex* freq_repr);

/**
 * @brief compute amplitude and phase spectrum from freqency domain
 * @param int height : the input frequency domain height
 * @param int width : the input frequency domain width
 * @param fft_complex* freq_repr : the frequency input 
 * @param float* as : the compute amplitude spectrum
 * @param float* ps : the compute phase spectrum 
 */
extern void 
fft_fr_to_spectra(int height, int width, 
		  fftw_complex* freq_repr,
		  float* as, float* ps);

/**
 * @brief compute the frequency domain from amplitude and phase spectrum
 * @param int height : the input frequency domain height
 * @param int width : the input frequency domain width
 * @param float* as : the amplitude spectrum input
 * @param float* ps : the phase spectrum input
 * @param fft_complex* freq_repr : the frequency output
 */
extern void 
fft_spectra_to_fr(int height, int width, 
		  float* as, float* ps,
		  fftw_complex* freq_repr);

#endif /* FFT_H */
