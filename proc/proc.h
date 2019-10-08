#ifndef PROC_H
#define PROC_H


#include <string.h>
#include "bmp.h"

int **kernel(int size);

void grayscale(bmp *img);

double threshold(bmp *img);

void binarization(bmp *img);

#endif
