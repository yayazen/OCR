#ifndef _NOISE_REDUCTION_H_
#define _NOISE_REDUCTION_H_


#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

GtkWidget load_image(char path);
void swap(int *a, int *b);
void bubbleSort(int arr[], int n);
void image_denoise(GdkPixbuf *image);


#endif

