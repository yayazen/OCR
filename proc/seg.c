#include "seg.h"

charSet* newCharSet(size_t height, size_t width){
	charSet* set = malloc(sizeof(charSet));
	if (set == NULL){
		fprintf(stderr, "newCharSet: could not allocate memory\n");
		return NULL;
	}
	set->head = NULL;
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

	elm->img = NULL;
	elm->next = NULL;
	return elm;
}

void freeCharSet(charSet* set){
	freeCharSetObj(set->head);
	free(set);
	return;
}

void freeCharSetObj(charSetObj* setObj){
	if (setObj == NULL)
		return;
	freeCharSetObj(setObj->next);
	freeImgObj(setObj->img);
	free(setObj);
	return;
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


//TODO move into proc.c or img.c
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


charSet* createCharSetFromLine(imgObj* img, size_t setHeight, size_t setWidth){
	if (img == NULL){
		fprintf(stderr, "createCharSetFromLine: img is NULL, could not create charSet\n");
		return NULL;
	}
	if (setHeight == 0 || setWidth == 0){
		fprintf(stderr, "createCharSetFromLine: incorrect set dimensions\n");
		return NULL;
	}
	
	// booleans
	int* stripType = calloc(img->w, sizeof(int));
	if (stripType == NULL){
		fprintf(stderr, "createCharSetFromLine: could not allocate memory\n");
		return NULL;
	}

	for (size_t w = 0; w < img->w; w++){
		for (size_t h = 0; h < img->h; h++){
			uint8_t* pix = getPixel(img, h, w);
			if (pix[0] == 0){
				stripType[w] = 1;
				break;
			}
		}
	}

#ifdef PRESENTATION
	
	for (size_t w = 0; w < img->w; w++){
		if (stripType[w] == 0){
			for (size_t h = 0; h < img->h; h++){
				uint8_t* pix = getPixel(img, h, w);
				pix[0] = 255;
				pix[1] = 0;
				pix[2] = 0;
			}
		}
	}

#endif	
	size_t count = 0;
        int prevStrip = 0;
        for (size_t w = 0; w < img->w; w++){
                if (prevStrip != stripType[w]){
                        if (prevStrip){
                                count++;
                        }
                } 
                prevStrip = stripType[w];
        }
	
	charSet* set = newCharSet(setHeight, setWidth);
	if (set == NULL){
		fprintf(stderr, "createCharSetFromLine: could not create charSet\n");
		free(stripType);
		return NULL;
	}


	prevStrip = 0;
	size_t stripWidth = 0;
	for (size_t w = 0; w < img->w; w++){

		if (prevStrip != stripType[w]){
			

			if (prevStrip == 0){
				stripWidth = 0;
			} else {
				imgObj* charImg = newImgObj(img->h, stripWidth);
				if (charImg == NULL){
					fprintf(stderr, "createCharSetFromLine: could not allocate img\n");
					freeCharSet(set);
					free(stripType);
					return NULL;
				}
				copyImgObj(img, charImg, 0, w - stripWidth);
				imgObj* cleanedCharImg = fitImage(charImg, set->h, set->w);
				freeImgObj(charImg);
				if (cleanedCharImg == NULL){
					fprintf(stderr, "createCharSetFromLine: could not resize img\n");
					freeCharSet(set);
					free(stripType);
					return NULL;
				}
				charSetObj* elm = pushNewCharSetObj(set);
				if (elm == NULL){
					fprintf(stderr, "createCharSetFromLine: could not create charSetObj\n");
                                        freeCharSet(set);
                                        free(stripType);
					freeImgObj(cleanedCharImg);
                                        return NULL;
				}
				elm->img = cleanedCharImg;
			}
		}
		stripWidth++;
		prevStrip = stripType[w];
	}
        
                
	free(stripType);
	return set;
}



