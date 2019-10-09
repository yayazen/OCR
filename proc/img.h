#ifndef IMG_H
#define IMG_H

#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

typedef struct img {

        size_t h, w;
        uint8_t *pixels;

} imgObj;

imgObj* newImgObj(size_t height, size_t width);

void freeImgObj(imgObj* img);

uint8_t* getPixel(imgObj* img, size_t height, size_t width);
uint8_t* getPixelIndex(imgObj* img, size_t index);

#endif
