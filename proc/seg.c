#include "seg.h"

charSet* new_charSet(size_t height, size_t width){
	charSet* set = malloc(sizeof(charSet));
	if (set == NULL){
		fprintf(stderr, "new_charSet: could not allocate memory\n");
		return NULL;
	}
	set->head = NULL;
	set->h = height;
	set->w = width;
	return set;
}

charSetObj* new_charSetObj(charSet* set){
	if (set == NULL){
		fprintf(stderr, "new_charSetObj: NULL set, could not create element\n");
		return NULL;
	}

	charSetObj* elm = malloc(sizeof(charSetObj));
	if (elm == NULL){
		fprintf(stderr, "new_charSetObj: could not allocate memory\n");
		return NULL;
	}	

	elm->img = NULL;
	elm->next = NULL;
	elm->character = 0;
	return elm;
}

void free_charSet(charSet* set){
	free_charSetObj(set->head);
	free(set);
	return;
}

void free_charSetObj(charSetObj* setObj){
	if (setObj == NULL)
		return;
	free_charSetObj(setObj->next);
	if (setObj->img != NULL){
		g_object_unref(setObj->img);
	}
	free(setObj);
	return;
}


charSetObj* push_new_charSetObj(charSet* set){
	if (set == NULL){
		fprintf(stderr, "push_new_charSetObj: NULL set, could not create element\n");
		return NULL;
	}
	
	charSetObj* elm = set->head;
	if (elm == NULL) {
		set->head = new_charSetObj(set);
		return set->head;
	}

	while (elm->next != NULL){
		elm = elm->next;
	}
	elm->next = new_charSetObj(set);
	return elm->next;
}

GdkPixbuf** split_img_into_lines(GdkPixbuf* img, size_t* n, uint8_t threshold){
	if (img == NULL){
		fprintf(stderr, "split_img_into_lines: img is NULL, could not split image\n");
		return NULL;
	}

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	// booleans
	int* linesType = calloc(pixbufHeight, sizeof(int));
	if (linesType == NULL){
		fprintf(stderr, "split_img_into_lines: could not allocate memory\n");
		return NULL;
	}
	
	for (size_t h = 0; h < pixbufHeight; h++){
		for (size_t w = 0; w < pixbufWidth; w++){
			if (pixels[h * rowstride + w * n_channels] < threshold){
				linesType[h] = 1;
				break;
			}
		}
	}
	
	size_t count = 0;
	int prevLine = 0;
	for (size_t h = 0; h < pixbufHeight; h++){
		if (prevLine != linesType[h]){
			if (prevLine){
				count++;
			}
		}
		prevLine = linesType[h];
	}
	
	GdkPixbuf** lines = calloc(count, sizeof(GdkPixbuf*));
	if (lines == NULL){
		fprintf(stderr, "split_img_into_lines: could not allocate memory\n");
		free(linesType);
		return NULL;
	}
	
	prevLine = 0;
	size_t index = 0;
	size_t lineCount = 0;
	for (size_t h = 0; h < pixbufHeight; h++){
		if (prevLine != linesType[h]){
			if (!prevLine){
				lineCount = 0;
			} else {
				lines[index] = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, pixbufWidth, lineCount);
				if (lines[index] == NULL){
					fprintf(stderr, "split_img_into_lines: NULL object\n");
					free(linesType);
					for (size_t i = 0; i < index; i++){
						g_object_unref(lines[i]);
					}
					free(lines);
					return NULL;
				}
				copy_GdkPixbuf(img, lines[index], h - lineCount, 0);
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
void copy_GdkPixbuf(GdkPixbuf* src, GdkPixbuf* dst, size_t height, size_t width){
        if (src == NULL || dst == NULL){
                fprintf(stderr, "copy_GdkPixbuf: NULL image parameters\n");
                return;
        }

        size_t src_height = gdk_pixbuf_get_height(src);
        size_t src_width = gdk_pixbuf_get_width(src);
        size_t src_rowstride = gdk_pixbuf_get_rowstride(src);
        size_t src_n_channels = gdk_pixbuf_get_n_channels(src);
        guchar* src_pixels = gdk_pixbuf_get_pixels(src);

        size_t dst_height = gdk_pixbuf_get_height(dst);
        size_t dst_width = gdk_pixbuf_get_width(dst);
        size_t dst_rowstride = gdk_pixbuf_get_rowstride(dst);
        guchar* dst_pixels = gdk_pixbuf_get_pixels(dst);



        if (dst_height + height > src_height || dst_width + width > src_width){
                fprintf(stderr, "copy_GdkPixbuf: invalid dimensions\n");
                return;
        }

        for (size_t h = height; h < height + dst_height; h++){
                memcpy(&dst_pixels[(h - height) * dst_rowstride],
                        &src_pixels[h * src_rowstride + width * src_n_channels],
                        src_n_channels * dst_width);
        }
}


charSet* create_charSet_from_line(GdkPixbuf* img, size_t setHeight, size_t setWidth, uint8_t threshold){
	if (img == NULL){
		fprintf(stderr, "create_charSet_from_line: img is NULL, could not create charSet\n");
		return NULL;
	}
	if (setHeight == 0 || setWidth == 0){
		fprintf(stderr, "create_charSet_from_line: incorrect set dimensions\n");
		return NULL;
	}

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);
	
	// booleans
	int* stripType = calloc(pixbufWidth, sizeof(int));
	if (stripType == NULL){
		fprintf(stderr, "create_charSet_from_line: could not allocate memory\n");
		return NULL;
	}

	for (size_t w = 0; w < pixbufWidth; w++){
		for (size_t h = 0; h < pixbufHeight; h++){
			uint8_t* pix = &pixels[h* rowstride + w * n_channels];
			if (pix[0] <= threshold){
				stripType[w] = 1;
				break;
			}
		}
	}

	size_t count = 0;
        int prevStrip = 0;
        for (size_t w = 0; w < pixbufWidth; w++){
                if (prevStrip != stripType[w]){
                        if (prevStrip){
                                count++;
                        }
                } 
                prevStrip = stripType[w];
        }
	
	charSet* set = new_charSet(setHeight, setWidth);
	if (set == NULL){
		fprintf(stderr, "create_charSet_from_line: could not create charSet\n");
		free(stripType);
		return NULL;
	}


	prevStrip = 0;
	size_t stripWidth = 0;
	for (size_t w = 0; w < pixbufWidth; w++){

		if (prevStrip != stripType[w]){
			

			if (prevStrip == 0){
				stripWidth = 0;
			} else {

				//all of that definitly needs some optimization, will come in due time			

				GdkPixbuf* charImg = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, stripWidth, pixbufHeight);
				if (charImg == NULL){
					fprintf(stderr, "create_charSet_from_line: could not allocate img\n");
					free_charSet(set);
					free(stripType);
					return NULL;
				}
				copy_GdkPixbuf(img, charImg, 0, w - stripWidth);

				size_t n = 0;
				GdkPixbuf** splitChars = split_touching_characters(charImg, &n, threshold);
				g_object_unref(charImg);

				for (size_t i = 0; i < n; i++){
					
					if (splitChars[i] == NULL){
						continue;
					}


					GdkPixbuf* cleanedCharImg = fit_image(splitChars[i], set->h, set->w);
                                	g_object_unref(splitChars[i]);
                                	if (cleanedCharImg == NULL){
                                	        fprintf(stderr, "create_charSet_from_line: could not resize img\n");
                                	        free_charSet(set);
                                	        free(stripType);
                                	        return NULL;
                                	}
                                	charSetObj* elm = push_new_charSetObj(set);
                                	if (elm == NULL){
                                        	fprintf(stderr, "create_charSet_from_line: could not create charSetObj\n");
                                        	free_charSet(set);
                                	        free(stripType);
                        	                g_object_unref(cleanedCharImg);
                	                        return NULL;
        	                        }
	                                elm->img = cleanedCharImg;

				}
				free(splitChars);
			}
		}
		stripWidth++;
		prevStrip = stripType[w];
	}
        
                
	free(stripType);
	return set;
}


void drop_fall(GdkPixbuf* img, uint8_t threshold){
	// initialize dropped pixels

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	for (size_t w = 0; w < pixbufWidth; w++){
		uint8_t* pix = &pixels[w * n_channels];
		if (pix[0] > threshold){
			pix[0] = 127;
		}
	}

	

	for (size_t h = 0; h < pixbufHeight - 1; h++){
		for (size_t w = 0; w < pixbufWidth; w++){
			uint8_t* pix = &pixels[h * rowstride + w * n_channels];
			if (pix[0] != 127){
				continue;
			}
	
			uint8_t* pixTested = &pixels[(h + 1) * rowstride + w * n_channels];
			if (pixTested[0] == 127){
				continue;
			}
			if (pixTested[0] > threshold){
				pixTested[0] = 127;
				continue;
			}

			int endRight = (w >= pixbufWidth -1);
			if (!endRight){
				pixTested = &pixels[(h + 1) * rowstride + (w + 1) * n_channels];
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
				pixTested = &pixels[(h + 1) * rowstride + (w - 1) * n_channels];
				if (pixTested[0] == 127){
					continue;
				}	
				if (pixTested[0] > threshold){
					pixTested[0] = 127;
					continue;
				}
			}

			if (!endRight){
				pixTested = &pixels[h * rowstride + (w + 1) * n_channels];
				if (pixTested[0] == 127){
					continue;
				}
				if (pixTested[0] > threshold){
					pixTested[0] = 127;
					continue;
				}
			}
			
			if (!endLeft){
				pixTested = &pixels[h * rowstride + (w - 1) * n_channels];
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

GdkPixbuf** split_touching_characters(GdkPixbuf* img, size_t *n, uint8_t threshold){
	drop_fall(img, threshold);
	*n = 1;
	int betweenChars = 0;

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img); 
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	for (size_t w = 0; w < pixbufWidth; w++){
		if (pixels[(pixbufHeight - 1) * rowstride + w * n_channels] == 127){
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
	GdkPixbuf** chars = calloc(*n, sizeof(GdkPixbuf*));
	betweenChars = 1;
	size_t i = 0;
        for (size_t w = 0; w < pixbufWidth; w++){
                if (pixels[(pixbufHeight - 1) * rowstride + w * n_channels] == 127){
                        if (!betweenChars){
                                betweenChars = 1;
                        }
                } else {
                        if (betweenChars){
                                betweenChars = 0;
				size_t minHeight = pixbufHeight - 1, maxHeight = pixbufHeight - 1, minWidth = w, maxWidth = w;
				finding_character(img, pixbufHeight - 1, w, &minHeight, &maxHeight, &minWidth, &maxWidth);
				chars[i] = copying_characters(img, minHeight, maxHeight, minWidth, maxWidth, threshold);
				i++;
                        }
                }
        }
	return chars;
}

//recursive function; could be otpimized by using a stack
void finding_character(GdkPixbuf* img, size_t h, size_t w, 
	size_t *minHeight, size_t *maxHeight, size_t *minWidth, size_t *maxWidth){
	
	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	if (h >= pixbufHeight || w >= pixbufWidth)
		return;
	uint8_t *pix = &pixels[h * rowstride + w * n_channels];
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
	
	finding_character(img, h+1, w, minHeight, maxHeight, minWidth, maxWidth);
	finding_character(img, h-1, w, minHeight, maxHeight, minWidth, maxWidth);
	finding_character(img, h, w+1, minHeight, maxHeight, minWidth, maxWidth);
	finding_character(img, h, w-1, minHeight, maxHeight, minWidth, maxWidth);
	return;
}

GdkPixbuf* copying_characters(GdkPixbuf* src, size_t minHeight, size_t maxHeight, 
	size_t minWidth, size_t maxWidth, uint8_t threshold){
	
	guchar* src_pixels = gdk_pixbuf_get_pixels(src);
        size_t src_rowstride = gdk_pixbuf_get_rowstride(src);
        size_t src_n_channels = gdk_pixbuf_get_n_channels(src);
	
	GdkPixbuf* dst = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, maxWidth - minWidth + 1, maxHeight - minHeight + 1);

	guchar* dst_pixels = gdk_pixbuf_get_pixels(dst);
        size_t dst_rowstride = gdk_pixbuf_get_rowstride(dst);
        size_t dst_n_channels = gdk_pixbuf_get_n_channels(dst);

	for (size_t h = minHeight; h <= maxHeight; h++){
		for (size_t w = minWidth; w <= maxWidth; w++){
			uint8_t* pix = &src_pixels[h * src_rowstride + w * src_n_channels];
			uint8_t* newPix = &dst_pixels[(h - minHeight) * dst_rowstride + (w - minWidth) * dst_n_channels];
			
			if (pix[0] == 128){
				pix[0] = 127;
				if (pix[1] > threshold){
					newPix[0] = 255;
					newPix[1] = 255;
					newPix[2] = 255;
				} else {
					newPix[0] = pix[1];
					newPix[1] = pix[1];
					newPix[2] = pix[1];
				}
			} else {
				newPix[0] = 255;
				newPix[1] = 255;
				newPix[2] = 255;
			}

		}
	}
	return dst;
}


charSet** segmentation(GdkPixbuf* img, size_t h, size_t w, uint8_t threshold, size_t *nLines){
	*nLines = 0;
	GdkPixbuf** lines = split_img_into_lines(img, nLines, threshold);
	charSet** sets = calloc(*nLines, sizeof(charSet*));
	
	
	for (size_t i = 0; i < *nLines; i++){
		sets[i] = create_charSet_from_line(lines[i], h, w, threshold);
		g_object_unref(lines[i]);
	}
	free(lines);

	return sets;
}


