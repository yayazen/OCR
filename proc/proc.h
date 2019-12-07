#ifndef PROC_H
#define PROC_H

#include <math.h>
#include <errno.h>
#include <string.h>
#include <gdk/gdk.h>

int **kernel(int size);

void grayscale(GdkPixbuf* img);

double threshold(GdkPixbuf* img);

void binarization(GdkPixbuf* img, double t);

GdkPixbuf* proximal_interpolation(GdkPixbuf* img, size_t height, size_t width);
// uses proximal interpolation to resize image without changing the aspect ratio,
// note that the output image will NOT always match height*width, use fit_image for this result.
// will return NULL if an error is encountered
// note that this is a lesser option for downscaling, may implement other algoritms in the future

GdkPixbuf* filling(GdkPixbuf* img, size_t height, size_t width);
// will add white pixels to an image to make it fit height*width
// will return NULL if an error is encountered
// will return the pointer to the input image if size is identical

GdkPixbuf* fit_image(GdkPixbuf* img, size_t height, size_t weight);
// fit_image makes an image fit into a h*w size image
// by compressing / decompressing and then adding pixels to fir the format
// will return NULL if an error is encountered

double hough(size_t* points, size_t len, double precision);

//TODO
size_t* findPointsOfInterest(GdkPixbuf* img, size_t n);



#endif
