/***********************************************************************
 * @file bitmap.c
 * @brief implements the functions of the bitmap structure
 * Course: CPE2600
 * Assignment: Lab 12 - Multithreading
 * Author: Zoya Mumtaz
 * Date: 11/11/2025
 * Note: compile with
 *     $ make
 *     run with
 *     $ ./movie
 ********************************************************************/

#include "bitmap.h"
#include <stdlib.h>


struct bitmap {
    int width;
    int height;
    
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    
    int max;
    
    imgRawImage *img;
 
    int threads;
};

/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
Moved to bitmap.c so bitmap_set() can utilize it
*/
static int iteration_to_color( int iters, int max )
{
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}


struct bitmap *bitmap_create(int width, int height, int max, imgRawImage *img, int threads) {
    //using dynamic memory to store bitmap struct to keep it alive
    struct bitmap *bm = malloc(sizeof(struct bitmap));

    bm->width = width;
    bm->height = height;
    bm->max = max;
    bm->threads = threads;

    bm->img = img;

    //default
    bm->xmin = 0;
    bm->xmax = 0;
    bm->ymin = 0;
    bm->ymax = 0;
    
    return bm;
}

void free_bitmap(struct bitmap *bm) {
    if (bm != NULL) {
        free(bm);
    }
}

//the getter functions
int bitmap_width(struct bitmap *bm) { 
    return bm->width; 
}
int bitmap_height(struct bitmap *bm) {
    return bm->height;
}
int bitmap_max(struct bitmap *bm) {
    return bm->max; 
}
int bitmap_threads(struct bitmap *bm) { 
    return bm->threads; 
}
double bitmap_xmin(struct bitmap *bm) { 
    return bm->xmin; 
}
double bitmap_xmax(struct bitmap *bm) { 
    return bm->xmax; 
}
double bitmap_ymin(struct bitmap *bm) { 
    return bm->ymin; 
}
double bitmap_ymax(struct bitmap *bm) { 
    return bm->ymax; 
}

struct imgRawImage *bitmap_raw(struct bitmap *bm) { 
    return bm->img; 
}

void bitmap_set(struct bitmap *bm, int x, int y, int iters) {
   int max = bm->max;
   int color = iteration_to_color(iters, max);
   //write to the raw image
   setPixelCOLOR(bm->img, y, x, color);
}