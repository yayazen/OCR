#include "img.h"



imgObj* newImgObj(size_t height, size_t width){
        imgObj* img = malloc(sizeof(imgObj));
        if (img == NULL){
                //-- Malloc Error --
        }

        img->h = height;
        img->w = width;
        img->pixels = calloc(3 * height * width, sizeof(uint8_t));

        if (img->pixels == NULL){
                //-- Malloc Error --
        }

        return img;
}

void freeImgObj(imgObj* img){
        free(img->pixels);
        free(img);
}

uint8_t* getPixel(imgObj* img, size_t height, size_t width){
	if ((height >= img->h) || (width >= img->w)){
		fprintf(stderr, "getPixel: out of bounds coordinates\n");
		return NULL;
	}
	return img->pixels + ((height*img->w + width) * 3);
}

uint8_t* getPixelIndex(imgObj* img, size_t index){
		if (index >= img->h * img->w){
			fprintf(stderr, "getPixelIndex: out of bounds coordinates\n");
			return NULL;
		}
	return &(img->pixels[index*3]);	
}

