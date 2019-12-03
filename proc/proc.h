#ifndef PROC_H
#define PROC_H

typedef struct {
	unsigned char *pixels;

	int height, width;
	int rowstride, n_chan;

} IMGData;

void grayscale(IMGData *data);
void binarize(IMGData *data);

#endif
