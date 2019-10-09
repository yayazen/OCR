#ifndef PROC_H
#define PROC_H

#include <math.h>
#include <errno.h>
#include <string.h>
#include "bmp.h"
#include "img.h"

int **kernel(int size);

void grayScale(imgObj* img);

double threshold(imgObj* img);

void binarization(imgObj* img);

double hough(size_t* points, size_t len, double precision);

size_t* findPointsOfInterest(imgObj* img, size_t n);

#endif
