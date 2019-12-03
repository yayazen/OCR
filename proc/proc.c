#include "proc.h"
#include <stdlib.h>

unsigned char treshold(IMGData *data) {
	unsigned char *p;
	float sum = 0.0f;
	for (int i = 0; i < data->height; i++) {
		for (int j = 0; j < data->width; j++) {
			p = data->pixels + i * data->rowstride + j * data->n_chan;
			sum += p[0];
		}
	}
	return sum / (data->height * data->width);
}

void binarize(IMGData *data) {
	unsigned char t = treshold(data);

	unsigned char b, *p;
	for (int i = 0; i < data->height; i++) {
		for (int j = 0; j < data->width; j++) {
			p = data->pixels + i * data->rowstride + j * data->n_chan;
			b = (p[0] > t) ? 255 : 0;
			p[0] = p[1] = p[2] = b;	
		}		
	}
}


void grayscale(IMGData *data) {
	unsigned char l, *p;
	for (int i = 0; i < data->height; i++) {
		for (int j = 0; j < data->width; j++) {
			p = data->pixels + i * data->rowstride + j * data->n_chan;
			l = 0.2126 * p[0] + 0.7152 * p[1] + 0.0722 * p[2];
	
			p[0] = p[1] = p[2] = l;
			
		}
	}
}

