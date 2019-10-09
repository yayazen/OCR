#include "bmp.h"
#include "img.h"
#include "seg.h"
#include "proc.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char** argv){
	if (argc < 2){
		fprintf(stderr, "main: invalid number of argument\n");
		return 0;
	}
	bmp* bmpImg = bmp_load(argv[1]);
	imgObj* img = bmpToImgObj(bmpImg);
	printf("%lu height, %lu width\n", img->h, img->w);
	//grayScale(img);
	//binarization(img);
	size_t n = 0;
	splitImgIntoLines(img, &n);
	printf("%lu lines\n", n);
	
	/*for (size_t h = 0; h < img->h; h++){
		uint8_t* pix = getPixel(img, h, 0);
		pix[1] = 0;
		pix[2] = 0;
	}
	
	for (size_t w = 0; w < img->w; w++){
		uint8_t* pix = getPixel(img, 10, w);
		pix[0] = 0;
		pix[2] = 0;
	}
	uint8_t* pix = getPixel(img, 12, 1);
	pix[0] = 0;
	pix[1] = 0;*/
	bmp* newBmpImg = imgObjToBmp(img, bmpImg->file_h, bmpImg->info_h);
	bmp_save(newBmpImg, "out.bmp");
	//bmp_save(bmpImg, "out.bmp");
	return 1;
}
