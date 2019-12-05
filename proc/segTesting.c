#include "seg.h"
#include "proc.h"
#include <stdlib.h>
#include <stdio.h>
#include <gdk/gdk.h>

// Use with ../data/project.bmp for argument
// Still room for improvements


int main(int argc, char** argv){
	if (argc < 2){
		fprintf(stderr, "main: invalid number of argument\n");
		return 1;
	}
	
	GError* err = NULL;
	GdkPixbuf *img = gdk_pixbuf_new_from_file(argv[1], &err);

	if (err != NULL){
		fprintf(stderr, "Could not load file\n");
		return 1;
	}

	// manual entry of threshold rather than calculating the average value makes for cleaner images,
	// although we need some sort of cleaner way for doing this as the user probably won't want to have to fiddle with
	// 100 different settings
	uint8_t threshold = 100;

	grayscale(img);
	binarization(img, 100);


	size_t n = 0;
	charSet** chars = segmentation(img, 64, 64, 100, &n);


	//File saving... strongly inefficient
	
	for (size_t i = 0; i < n; i++){
		size_t j = 0;
		charSetObj* c = chars[i]->head;
		while (c != NULL){

			char *s = calloc(100, sizeof(char));
			sprintf(s, "out/%03lu%03lu.bmp", i, j);
			
			gdk_pixbuf_save(c->img, s, "bmp", &err, NULL);
			if (err != NULL){
				fprintf(stderr, "Could not save file %s\n", s);
				
				
				
				return 1;
			}

			c = c->next;
			j++;
			free(s);
		}
		free_charSet(chars[i]);
	}
	free(chars);
	g_object_unref(img);

	return 0;
}
