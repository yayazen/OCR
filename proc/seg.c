#include "seg.h"

charSet* newCharSet(size_t height, size_t width){
	charSet* set = malloc(sizeof(charSet));
	if (set == NULL){
		fprintf(stderr, "newCharSet: could not allocate memory\n");
		return NULL;
	}

	set->h = height;
	set->w = width;
	return set;
}

charSetObj* newCharSetObj(charSet* set){
	if (set == NULL){
		fprintf(stderr, "newCharSetObj: NULL set, could not create element\n");
		return NULL;
	}

	charSetObj* elm = malloc(sizeof(charSetObj));
	if (elm == NULL){
		fprintf(stderr, "newCharSetObj: could not allocate memory\n");
		return NULL;
	}	

	elm->img = newImgObj(set->h, set->w);
	elm->next = NULL;
	return elm;
}

charSetObj* pushNewCharSetObj(charSet* set){
	if (set == NULL){
		fprintf(stderr, "pushNewCharSetObj: NULL set, could not create element\n");
		return NULL;
	}
	
	charSetObj* elm = set->head;
	if (elm == NULL) {
		set->head = newCharSetObj(set);
		return set->head;
	}
	while (elm->next != NULL){
		elm = elm->next;
	}
	elm->next = newCharSetObj(set);
	return elm->next;
}

imgObj** splitImgIntoLines(imgObj* img, size_t* n){
	if (img == NULL){
		fprintf(stderr, "splitImgIntoLines: img is NULL, could not split image\n");
		return NULL;
	}
	
	// booleans
	int* linesType = calloc(img->h,sizeof(int));
	if (linesType == NULL){
		fprintf(stderr, "splitImgIntoLines: could not allocate memory\n");
		return NULL;
	}
	
	for (size_t h = 0; h < img->h; h++){
		for (size_t w = 0; w < img->w; w++){
			if (getPixel(img, h, w)[0] == 0){
				linesType[h] = 1;
				break;
			}
		}
	}
#ifdef PRESENTATION
	printf("Writing img...\n");
	for (size_t h = 0; h < img->h; h++){
		if (!linesType[h]){
			for (size_t w = 0; w < img->w; w++){
				getPixel(img, h, w)[1] = 0;
				getPixel(img, h, w)[2] = 0;
			}
		}
	}

#endif
	
	size_t count = 0;
	int prevLine = 0;
	for (size_t h = 0; h < img->h; h++){
		if (prevLine != linesType[h]){
			if (prevLine){
				count++;
			}
		}
		prevLine = linesType[h];
	}
#ifdef DEBUG

	for (size_t h = 0; h < img->h; h++){
		printf("line %lu type : %i\n", h, linesType[h]);
	}
#endif
	imgObj** lines = calloc(count, sizeof(imgObj*));
	if (lines == NULL){
		fprintf(stderr, "splitImgIntoLines: could not allocate memory\n");
		free(linesType);
		return NULL;
	}
	
	prevLine = 0;
	size_t index = 0;
	size_t lineCount = 0;
	for (size_t h = 0; h < img->h; h++){
		if (prevLine != linesType[h]){
			if (!prevLine){
				lineCount = 0;
			} else {
				lines[index] = newImgObj(lineCount, img->w);
				if (lines[index] == NULL){
					fprintf(stderr, "splitImgIntoLines: NULL object\n");
					free(linesType);
					for (size_t i = 0; i < index; i++){
						freeImgObj(lines[i]);
					}
					free(lines);
					return NULL;
				}
				copyImgObj(img, lines[index], h - lineCount, 0);
				index++;
			}
		}
		if (linesType[h]){
			lineCount++;
		}
		prevLine = linesType[h];
	}
	
	free(linesType);
	*n = count;
	return lines;
}

void copyImgObj(imgObj* src, imgObj* dst, size_t height, size_t width){
	if (src == NULL || dst == NULL){
		fprintf(stderr, "copyImgObj: NULL image parameters\n");
		return;
	}
	
	if (dst->h + height > src->h || dst->w + width > src->w){
		fprintf(stderr, "copyImgObj: invalid dimensions\n");
		return;
	}

	for (size_t h = height; h < height + dst->h; h++){
		memcpy(getPixel(dst, h - height, 0), getPixel(src, h, width), 3 * sizeof(uint8_t) * dst->w);
	}
}


// WIP unimplemented for now
charSet* createCharSetFromLine(imgObj* img, size_t height, size_t startWidth, size_t endWidth, size_t setHeight, size_t setWidth){
	if (img == NULL){
		fprintf(stderr, "createCharSetFromLine: img is NULL, could not create charSet\n");
		return NULL;
	}
	if (startWidth > endWidth){
		fprintf(stderr, "createCharSetFromLine: startWidth should not be > to endWidth\n");
		return NULL;
	}
	if (img->h < height || img->w < startWidth || img->w < endWidth){
		fprintf(stderr, "createCharSetFromLine: out of bound coordinates\n");
		return NULL;
	}
	if (setHeight == 0 || setWidth == 0){
		fprintf(stderr, "createCharSetFromLine: incorrect set dimensions\n");
	}
	fprintf(stderr, "createCharSetFromLine: Unimplemented Function\n");
	return NULL;
}
