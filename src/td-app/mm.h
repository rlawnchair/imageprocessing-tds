#ifndef __MM_HH__
#define __MM_HH__

/**
 *  @file  mm.h
 *  @brief header for mm.c thats implements functions for mathematical image 
 *         image processing, in particular structuring elements.
 *	   A structuring element is viewed as a pnm binary image file : 0 for
 *	   the background and 255 for the shape.
 */
#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

/**
 * @brief  generate a structuring element of a given shape and halfsize
 * @param  shape: the structing element shape umber
 *         0: square
 *         1: diamond
 *	   2: disk
 * @param  halfsize: the structuring element halfsize
 * @return the structuring element of (2*halfsize+1)^2 size as an pnm object
 */
pnm 
mm_se(int shape, int halfsize); 

/**
 * @brief  compute a morphological dilation or erosion on a grayscale image
 *         with a given structuring element. Dilation or erosion 
 *         processing depends on an order function  defined by the pointer pf
 * @param  shape: the structing element shape umber
 * @param  halfsize: the structuring element halfsize
 * @param  ims: the input image source to process
 * @param  imd: the destination image
 * @param  pf: a pointer on a ordering function
 */
void
mm_process(int shape, 
	   int halfsize, 
	   pnm ims, 
	   pnm imd, 
	   void (*pf)(unsigned short, unsigned short*));

/**
 * @brief  ordering function, if val is geater than max then update max
 * @param  val: the input value
 * @param  max: the value to update
 */
void
mm_greater(unsigned short val, unsigned short* max);

/**
 * @brief  ordering function, if val is lesser than min then update min
 * @param  val: the input value
 * @param  min: the value to update
 */
void
mm_lesser(unsigned short val, unsigned short* min);

#endif
