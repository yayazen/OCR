//include "testdenoise.h"
#include <stdlib.h>
#include <gtk/gtk.h>

void swap(int *a, int *b)
{
    int c = *b;
    *b = *a;
    *a = c;
}
void bubbleSort(int arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)
   {
       for (j = 0; j < n-i-1; j++)
       {
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
       }
    }
}

void denoise(GdkPixbuf *image)
{
    unsigned int len;
    guchar* pixels = gdk_pixbuf_get_pixels_with_length(image, &len);

     
    int width = gdk_pixbuf_get_width(image);
    int height = gdk_pixbuf_get_height(image);
    int rowstride = gdk_pixbuf_get_rowstride(image);
    int n_channels = gdk_pixbuf_get_n_channels(image);


    GdkPixbuf *new = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    guchar* px = gdk_pixbuf_get_pixels(new); 

    int mediane;

    for (int w = 1; w < (width - 1); w++)
    {
        for (int h = 1; h < (height - 1); h++)
        {
            int pixelarray[8];
            pixelarray[0] = pixels[(h-1) * rowstride + (w-1) * n_channels];
            pixelarray[1] = pixels[(h)   * rowstride + (w-1) * n_channels];
            pixelarray[2] = pixels[(h-1) * rowstride + (w)   * n_channels];
            pixelarray[3] = pixels[(h-1) * rowstride + (w+1) * n_channels];
            pixelarray[4] = pixels[(h)   * rowstride + (w+1) * n_channels];
            pixelarray[5] = pixels[(h+1) * rowstride + (w-1) * n_channels];
            pixelarray[6] = pixels[(h+1) * rowstride + (w)   * n_channels];
            pixelarray[7] = pixels[(h+1) * rowstride + (w+1) * n_channels];

            bubbleSort(pixelarray,7);
            mediane = pixelarray[3];
            px[h * rowstride + w * n_channels] = mediane;
            px[h * rowstride + w * n_channels + 1] = mediane;
            px[h * rowstride + w * n_channels + 2] = mediane;


        }
    }
    
    memcpy(pixels, px, len);
}
