/***********************************************************************
 * @file bitmap.h
 * @brief declares the bitmap structure
 * Course: CPE2600
 * Assignment: Lab 12 - Multithreading
 * Author: Zoya Mumtaz
 * Date: 11/11/2025
 * Note: compile with
 *     $ make
 *     run with
 *     $ ./movie
 ********************************************************************/

#ifndef BITMAP_H
#define BITMAP_H

#include "jpegrw.h"

struct bitmap;

//constructor (allocate) function
struct bitmap *bitmap_create(int width, int height, int max, imgRawImage *img, int threads);
//destructor (deallocate) function
void free_bitmap(struct bitmap *bm);

//getters
int bitmap_width(struct bitmap *bm);
int bitmap_height(struct bitmap *bm);
int bitmap_max(struct bitmap *bm);
int bitmap_threads(struct bitmap *bm);

double bitmap_xmin(struct bitmap *bm);
double bitmap_xmax(struct bitmap *bm);
double bitmap_ymin(struct bitmap *bm);
double bitmap_ymax(struct bitmap *bm);

//setter
void bitmap_set(struct bitmap *bm, int x, int y, int iters);

//access the raw image if needed
struct imgRawImage *bitmap_raw(struct bitmap *bm);

#endif
