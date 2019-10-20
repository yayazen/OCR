#ifndef SEG_H
#define SEG_H

#include "img.h"
#include "proc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct charSetObj {
	struct charSetObj* next;
	// next element of linked list
	imgObj* img;
	// image of the character object
} charSetObj;

typedef struct charSet {
        size_t h, w;
	// height and width of element's images
        charSetObj* head;
	// head of the linked list
} charSet;

charSet* newCharSet(size_t height, size_t width);
// creates new linked list, all elements will have an image of height*width dimension
// return pointer to charSet object
// return NULL if no set could be created
// NB: does not create any element, charSet->next will be set to NULL

void freeCharSet(charSet* set);

void freeCharSetObj(charSetObj* setObj);

charSetObj* newCharSetObj(charSet* set);
// creates new element according to "set"'s dimensions
// returns pointer to created element
// returns NULL if no element could be created
// NB: does not modify the "set" linked list

charSetObj* pushNewCharSetObj(charSet *set);
// creates new element and pushes it at the end of the linked list "set"
// returns pointer to created element
// returns NULL if no element could be created and set is unchanged

imgObj** splitImgIntoLines(imgObj* img, size_t* n);
// Splits the image into image of lines of text, for each image, only a single line will be present
// returns a pointer to an array of pointer to imaObj
// returns NULL when an error is encountered

//TODO move into proc.c or img.c
void copyImgObj(imgObj* src, imgObj* dst, size_t height, size_t width);
// copies an area of dst->h * dst->w from src, starting from height,width
// note that the whole area must be inside src, if dimensions are out of bounds, 
// the function will return with nothing changed.


//TODO
charSet* createCharSetFromLine(imgObj* img, size_t setHeight, size_t setWidth);
// this function reads and imgObj and creates a charSet linked list out of the characters found on that line.
// "setHeigh" is the height of the image of the elements of charSet
// "setWidth" is the width of the image of the emelents of charSet
// this function assumes that the image is binarized, black for character pixels and white for background.
// return a pointer to a charSet linked list
// return NULL if an error is encountered

#endif
