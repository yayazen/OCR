#ifndef SEG_H
#define SEG_H

#include "proc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <gdk/gdk.h>

#define SEG_TAB_SPACE_EQ 4

typedef struct charSetObj {
	struct charSetObj* next;
	// next element of linked list
	GdkPixbuf* img;
	char character;
	size_t npix;
	// image of the character object
} charSetObj;

typedef struct charSet {
        size_t h, w;
	// height and width of element's images
        charSetObj* head;
	charSetObj* tail;
	// head of the linked list
} charSet;

charSet* new_charSet(size_t height, size_t width);
// creates new linked list, all elements will have an image of height*width dimension
// return pointer to charSet object
// return NULL if no set could be created
// NB: does not create any element, charSet->next will be set to NULL

void free_charSet(charSet* set);

void free_charSetObj(charSetObj* setObj);

charSetObj* new_charSetObj();
// creates new element according to "set"'s dimensions
// returns pointer to created element
// returns NULL if no element could be created
// NB: does not modify the "set" linked list

charSetObj* push_new_charSetObj(charSet *set);
// creates new element and pushes it at the end of the linked list "set"
// returns pointer to created element
// returns NULL if no element could be created and set is unchanged

GdkPixbuf** split_img_into_lines(GdkPixbuf* img, size_t* n, uint8_t threshold);
// Splits the image into image of lines of text, for each image, only a single line will be present
// returns a pointer to an array of pointer to GdkPixbuf
// returns NULL when an error is encountered

//TODO move into proc.c
void copy_GdkPixbuf(GdkPixbuf* src, GdkPixbuf* dst, size_t height, size_t width);
// copies an area of the height and width of dst from src, starting from height,width
// note that the whole area must be inside src, if dimensions are out of bounds, 
// the function will return with nothing changed.

charSet* create_charSet_from_line(GdkPixbuf* img, size_t setHeight, size_t setWidth, uint8_t threshold);
// this function reads the img line and creates a charSet linked list out of the characters found on that line.
// "setHeigh" is the height of the image of the elements of charSet
// "setWidth" is the width of the image of the emelents of charSet
// this function assumes that the image is binarized or grayscaled, with threshold as the threshold value between which a pixel is considered background or character, (if pixel > threshold ? background : character)
// return a pointer to a charSet linked list
// return NULL if an error is encountered

void drop_fall(GdkPixbuf* img, uint8_t treshold);
// this algorithms attempts to split touching characters
// output is a list of characters images

GdkPixbuf** split_touching_characters(GdkPixbuf* img, size_t* n, uint8_t threshold);
// splits the touching characters in an image using drop_fall. 

void finding_character(GdkPixbuf* img, size_t h, size_t w, 
	size_t *minHeight, size_t *maxHeight, size_t *minWidth, size_t *maxWidth);
// helps identifying the boundaries of a character in an imaged passed through drop_fall

GdkPixbuf* copying_characters(GdkPixbuf* src, size_t minHeight, size_t maxHeight, 
	size_t minWidth, size_t maxWidth, uint8_t threshold);
// copies a character from an imaged passed through finding_characters and drop_fall into a clean image

void convert_pix_len_chars(charSet* set);
// converts empty characters to space and tab (tab only for the first character if lenght is >= SEG_TAB_SPACE_EQ)

charSet** segmentation(GdkPixbuf* img, size_t h, size_t w, uint8_t threshold, size_t* nLines);
// Complete segmentation of an img.
// argument h and w define the size of the output character images.
// threshold defines the value beyond which a pixel is considered background, useful for grayscale images
// returns a list of charSet* of nLines's size. nLines is set to the number of lines found in the image.

#endif
