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
	//grayScale(img);
	//binarization(img);

	size_t n = 0;
	imgObj** lines = splitImgIntoLines(img, &n, 127);
	printf("%lu lines\n", n);
	charSet** sets = calloc(n, sizeof(charSet*));
	
	for (size_t i = 0; i < n; i++){
		sets[i] = createCharSetFromLine(lines[i], 28, 28, 127);
	}

	bmp* lineBmp = imgObjToBmp(img, bmpImg->file_h, bmpImg->info_h);
	bmp_save(lineBmp, "line.bmp");


	imgObj* tempImg = lines[3];
	bmp* charBmp = imgObjToBmp(tempImg, bmpImg->file_h, bmpImg->info_h);
	charBmp->info_h.width = tempImg->w;
	charBmp->info_h.height = tempImg->h;
	charBmp->file_h.size = tempImg->w * tempImg->h * 3;
	bmp_save(charBmp, "char.bmp");

	tempImg = sets[3]->head->img;
	bmp* charParsedBmp = imgObjToBmp(tempImg, bmpImg->file_h, bmpImg->info_h);
        charParsedBmp->info_h.width = tempImg->w;
        charParsedBmp->info_h.height = tempImg->h;
        charParsedBmp->file_h.size = tempImg->w * tempImg->h * 3;
        bmp_save(charParsedBmp, "charParsed.bmp");
	return 1;
}
