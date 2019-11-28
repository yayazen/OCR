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

imgObj** splitImgIntoLines(imgObj* img, size_t* n, uint8_t threshold){
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
			if (getPixel(img, h, w)[0] < threshold){
				linesType[h] = 1;
				break;
			}
		}
	}
	
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


charSet* createCharSetFromLine(imgObj* img, size_t setHeight, size_t setWidth, uint8_t threshold){
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
			if (pix[0] <= threshold){
				stripType[w] = 1;
				break;
			}
		}
	}

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

				//all of that definitly needs some optimization, will come in due time			

				imgObj* charImg = newImgObj(img->h, stripWidth);
				if (charImg == NULL){
					fprintf(stderr, "createCharSetFromLine: could not allocate img\n");
					freeCharSet(set);
					free(stripType);
					return NULL;
				}
				copyImgObj(img, charImg, 0, w - stripWidth);

				size_t n = 0;
				imgObj** splitChars = split_touching_characters(charImg, &n, threshold);
				freeImgObj(charImg);

				for (size_t i = 0; i < n; i++){

					imgObj* cleanedCharImg = fitImage(splitChars[i], set->h, set->w);
                                	freeImgObj(splitChars[i]);
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
		}
		stripWidth++;
		prevStrip = stripType[w];
	}
        
                
	free(stripType);
	return set;
}


void drop_fall(imgObj* img, uint8_t threshold){
	// initialize dropped pixels
	for (size_t w = 0; w < img->w; w++){
		if (getPixel(img, img->h-1, w)[0] > threshold){
			getPixel(img, img->h-1, w)[0] = 127;
		}
	}
	for (size_t h = img->h - 1; h > 0; h--){
		for (size_t w = 0; w < img->w; w++){
			uint8_t* pix = getPixel(img, h, w);
			if (pix[0] != 127){
				continue;
			}
	
			uint8_t* pixTested = getPixel(img, h-1, w);
			if (pixTested[0] == 127){
				continue;
			}
			if (pixTested[0] > threshold){
				getPixel(img, h-1, w)[0] = 127;
				continue;
			}

			int endRight = (w >= img->w -1);
			if (!endRight){
				pixTested = getPixel(img, h-1, w+1);
				if (pixTested[0] == 127){
					continue;
				}
				if (pixTested[0] > threshold){
					pixTested[0] = 127;
					continue;
				}
			}
			
			int endLeft = (w == 0);
			if (!endLeft){
				pixTested = getPixel(img, h-1, w-1);
				if (pixTested[0] == 127){
					continue;
				}	
				if (pixTested[0] > threshold){
					pixTested[0] = 127;
					continue;
				}
			}

			if (!endRight){
				pixTested = getPixel(img, h, w+1);
				if (pixTested[0] == 127){
					continue;
				}
				if (pixTested[0] > threshold){
					pixTested[0] = 127;
					continue;
				}
			}
			
			if (!endLeft){
				pixTested = getPixel(img, h, w-1);
				if (pixTested[0] == 127){
					continue;
				}
				if (pixTested[0] > threshold){
					pixTested[0] = 127;
					w -= 2;
					continue;
				}
			}
			
			//getPixel(img, h-1, w)[0] = 127;
		}
	}
}

imgObj** split_touching_characters(imgObj* img, size_t *n, uint8_t threshold){
	drop_fall(img, threshold);
	*n = 1;
	int betweenChars = 0;
	for (size_t w = 0; w < img->w; w++){
		if (getPixel(img, 0, w)[0] == 127){
			if (!betweenChars){
				betweenChars = 1;
				*n += 1;
			}
		} else {
			if (betweenChars){
				betweenChars = 0;
			}
		}
	}
	imgObj** chars = calloc(*n, sizeof(imgObj*));
	betweenChars = 1;
	size_t i = 0;
        for (size_t w = 0; w < img->w; w++){
                if (getPixel(img, 0, w)[0] == 127){
                        if (!betweenChars){
                                betweenChars = 1;
                        }
                } else {
                        if (betweenChars){
                                betweenChars = 0;
				size_t minHeight = 0, maxHeight = 0, minWidth = w, maxWidth = w;
				__finding_character(img, 0, w, &minHeight, &maxHeight, &minWidth, &maxWidth);
				chars[i] = copying_characters(img, minHeight, maxHeight, minWidth, maxWidth);
				i++;
                        }
                }
        }
	return chars;
}

//recursive function; could be otpimized by using a stack
void __finding_character(imgObj* img, size_t h, size_t w, 
	size_t *minHeight, size_t *maxHeight, size_t *minWidth, size_t *maxWidth){
	
	if (h >= img->h || w >= img->w)
		return;
	uint8_t *pix = getPixel(img, h, w);
	if (pix[0] == 127 || pix[0] == 128)
		return;
	pix[0] = 128;
	if (h > *maxHeight){
		*maxHeight = h;
	} else if (h < *minHeight){
		*minHeight = h;
	}
	if (w > *maxWidth){
		*maxWidth = w;
	} else if (w < *minWidth){
		*minWidth = w;
	}
	
	__finding_character(img, h+1, w, minHeight, maxHeight, minWidth, maxWidth);
	__finding_character(img, h-1, w, minHeight, maxHeight, minWidth, maxWidth);
	__finding_character(img, h, w+1, minHeight, maxHeight, minWidth, maxWidth);
	__finding_character(img, h, w-1, minHeight, maxHeight, minWidth, maxWidth);
	return;
}

imgObj* copying_characters(imgObj* src, size_t minHeight, size_t maxHeight, 
	size_t minWidth, size_t maxWidth){
	

	imgObj* dst = newImgObj(maxHeight - minHeight + 1, maxWidth - minWidth + 1);

	for (size_t h = minHeight; h <= maxHeight; h++){
		for (size_t w = minWidth; w <= maxWidth; w++){
			uint8_t* pix = getPixel(src, h, w);
			uint8_t* newPix = getPixel(dst, h - minHeight, w - minWidth);
			
			if (pix == NULL || newPix == NULL){
				fprintf(stderr, "copying_characters: wrong dimensions\n");
				return NULL;
			}
		
			if (pix[0] == 128){
				pix[0] = 127;
				newPix[0] = pix[1];
				newPix[1] = pix[1];
				newPix[2] = pix[1];
			} else {
				newPix[0] = 255;
				newPix[1] = 255;
				newPix[2] = 255;
			}

		}
	}
	return dst;
}


charSet** segmentation(imgObj* img, size_t h, size_t w, uint8_t threshold, size_t *nLines){
	*nLines = 0;
	imgObj** lines = splitImgIntoLines(img, nLines, threshold);
	charSet** sets = calloc(*nLines, sizeof(charSet*));
	
	for (size_t i = 0; i < *nLines; i++){
		sets[i] = createCharSetFromLine(lines[i], h, w, threshold);
	}

	return sets;
}


