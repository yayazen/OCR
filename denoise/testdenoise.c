#include "testdenoise.h"
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
              swap(&arr[j], &arr[j+1]);}}
}
void image_denoise(GdkPixbuf *image)
{
    guchar* pixels = gdk_pixbuf_get_pixels(image);
    int width = gdk_pixbuf_get_width(image);
    int height = gdk_pixbuf_get_height(image);
    int rowstride = gdk_pixbuf_get_rowstride(image);
    int n_channels = gdk_pixbuf_get_n_channels(image);

    int mediane;
    for (size_t w = 1; w < width - 1; w++)
    {
        for (size_t h = 1; h < height - 1; h++)
        {
            int pixelarray[7];
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
            pixels[h * rowstride + w * n_channels] = mediane;

        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    GdkPixbuf *pixel = gdk_pixbuf_new_from_file (argv[1], NULL);
    image_denoise(pixel);

    gdk_pixbuf_save(pixel, "out.jpg", "jpg", NULL, NULL);
    return 0;
}
