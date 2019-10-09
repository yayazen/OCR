#include "proc.h"

#define PI 3.141592

int **kernel(int size) {
	int **k = malloc(size * sizeof(int *));
	for(int r = 0; r < size; r++) {
		k[r] = malloc(size * sizeof(int));
		memset(k[r], 1, size);
	}	
	return k;
}



void grayScale(imgObj* img){
	if (img == NULL){
		fprintf(stderr, "grayScale: NULL image\n");
		return;
	}

	uint8_t v = 0;
	
	for (size_t h = 0; h < img->h; h++){
		for (size_t w = 0; w < img->w; w++){
			uint8_t* pix = getPixel(img, h, w);
			v = (uint8_t) 0.2126 * pix[0] + 0.7152 * pix[1] + 0.0722 * pix[2];
			pix[0] = v;
			pix[1] = v;
			pix[2] = v;
		}
	}
}

double threshold(imgObj* img){
	if (img == NULL){
		fprintf(stderr, "threshold: NULL image\n");
		return NAN;
	}

	double sum = 0;
	for (size_t h = 0; h < img->h; h++){
		for (size_t w = 0; w < img->w; w++){
			sum += getPixel(img, h, w)[0];
		}
	}
	return sum / (img->h * img->w);
}

void binarization(imgObj* img){
	if (img == NULL){
		fprintf(stderr, "binarization: NULL imgae\n");
		return;
	}
	double t = threshold(img);
	for (size_t h = 0; h < img->h; h++){
		for (size_t w = 0; w < img->w; w++){
			uint8_t* pix = getPixel(img, h, w);
			uint8_t p = (pix[0] > t) ? 255 : 0;
			pix[0] = p;
			pix[1] = p;
			pix[2] = p;
		}
	}
}

double hough(size_t* points, size_t len, double precision){
	size_t angles = (size_t) 360/precision;
	double* avrg = calloc(angles, sizeof(double));
	if (avrg == NULL) {
		errno = 1001;
		fprintf(stderr, "Hough: could not allocate memory\n");
		return NAN;
	}
	double* k = calloc(angles * len, sizeof(double));
	if (k == NULL) {
                errno = 1002;
                fprintf(stderr, "Hough: could not allocate memory\n");
		free(avrg);
                return NAN;
        }
	double* range = calloc(angles, sizeof(double));
	if (range == NULL) {
                errno = 1003;
                fprintf(stderr, "Hough: could not allocate memory\n");
		free(k);
		free(avrg);
                return NAN;
        }
	size_t minRange = angles;
	
	for (size_t i = 0; i < len; i++){
		for (size_t teta = 0; teta < angles; teta++){
			k[i * angles + teta] = sqrt( points[2*i] * points[2*i] + points[2*i+1] + points[2*i+1])\
				* sin(teta * precision * PI / 180.0);
			avrg[teta] += k[i * angles + teta];
		}
	}
	
	for (size_t teta = 0; teta < angles; teta++){
		avrg[teta] /= angles;
		for (size_t i = 0; i < len; i++){
			double v = k[i * angles + teta] - avrg[teta];
			range[teta] += (v >= 0)? v : -v;
		}
		if ((minRange == angles) || (range[teta] < range[minRange]))
			minRange = teta;
	}
	free(avrg);
	free(k);
	free(range);
	return minRange * precision;
}

size_t* findPointsOfInterest(imgObj* img, size_t n){
	if (img == NULL){
		fprintf(stderr,"findPointsOfInterest: imgObj is NULL, could not proceed\n");
		return NULL;
	}
	size_t* points = calloc(2 * n, sizeof(size_t));
	if (points == NULL) {
                errno = 2001;
                fprintf(stderr, "findPointsOfInterest: could not allocate memory\n");
		return NULL;
        }
	

	fprintf(stderr, "findPointsOdInterest: Unimplemented Function");
	errno = 2000;
	return NULL;
	
}

/*
void findAngle(bmp* img) {
	
	for (size_t y = 0; y < img.h; y++) {
		for (size_t x = 0; x < img.w; x++) {
			
		}
	}

}*/
