#include "proc.h"


int **kernel(int size) {
	int **k = malloc(size * sizeof(int *));
	for(int r = 0; r < size; r++) {
		k[r] = malloc(size * sizeof(int));
		memset(k[r], 1, size);
	}	
	return k;
}



void grayscale(bmp *img) {
	uint8_t l;
	
	for(int rw = 0; rw < img->h; rw++){
		for(int cl = 0; cl < img->w; cl++) {
			l = (uint8_t) 0.2126 * img->pixels[rw][cl].r + 0.7152 * img->pixels[rw][cl].g + 0.0722 * img->pixels[rw][cl].b;
			
			img->pixels[rw][cl] = (pixel){ .r = l, .g = l, .b = l };
		}
	}
}


double threshold(bmp *img) {
	double sum = 0;
	for(int rw = 0; rw < img->h; rw++) {
		for(int cl = 0; cl < img->w; cl++) {
			sum += img->pixels[rw][cl].r;
		}
	}
	
	return sum / (img->h * img->w);
}

void binarization(bmp *img) {
	double t = threshold(img);
	uint8_t b;
	for(int rw = 0; rw < img->h; rw++){
		for(int cl = 0; cl < img->w; cl++) {
			b = (img->pixels[rw][cl].r > t) ? 255 : 0;
			img->pixels[rw][cl] = (pixel) { .r = b, .g = b, .b = b }; 
		}
	}

}
