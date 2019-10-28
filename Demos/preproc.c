#include "bmp.h"
#include "proc.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage : ./preproc image.bmp");
		return 1;
	}

	
	bmp *img = bmp_load(argv[1]);
	if (!img)
		return errno;

	
	grayscale(img);
	
	bmp_save(img, "gray.bmp");
	
	binarization(img);
	bmp_save(img, "bin.bmp");

	bmp_close(img);
	return 0;
}
