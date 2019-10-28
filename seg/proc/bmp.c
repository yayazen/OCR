#include "bmp.h"

size_t fpread(void *buf, size_t size, size_t offset, FILE *fp) {
	if (fseek(fp, offset, SEEK_SET) != 0)
		return 0;
	return fread(buf, size, 1, fp);
}

void bmp_close(bmp *img) {
	for (int r = 0; r < img->h; r++)
		free(img->pixels[r]);
	free(img->pixels);
	img = NULL;
}

bmp * bmp_load(const char *path) {
	static bmp img;

	FILE *fp = fopen(path, "rb");
	
	if (!fp) {
		errno = FILE_ERROR;
		return NULL;
	}

	if (!fread(&img.file_h, sizeof(bmp_file_h), 1, fp) || !fread(&img.info_h, sizeof(bmp_info_h), 1, fp)) {
		fclose(fp);
		errno = INVALID_FORMAT;
		return NULL;
	}
	
	fseek(fp, img.file_h.offset, SEEK_SET);
#ifdef DEBUG
	bmp_file_h fh = img.file_h;
	printf("---------------\n"
		"bmpFileHeader:\n"
		"type = %i\n"
		"size = %u\n"
		"reserved = %u\n"
		"offset = %u\n",\
		fh.type,\
		fh.size,\
		fh.reserved,\
		fh.offset);
	
	bmp_info_h ih = img.info_h;
	printf("--------------\n"
		"bmpInfoHeader:\n"
		"bstruct_size = %i\n"
		"width = %u\n"
		"height = %u\n"
		"bplanes = %i\n"
		"bitcount = %i\n"
		"bcompression = %u\n"
		"img_size = %u\n"
		"Pxmeter_x = %u\n"
		"Pxmeter_y = %u\n"
		"bColorUsed = %u\n"
		"bColorImp = %u\n",\
		ih.bstruct_size,\
		ih.width,\
		ih.height,\
		ih.bplanes,\
		ih.bitcount,\
		ih.bcompression,\
		ih.img_size,\
		ih.Pxmeter_x,\
		ih.Pxmeter_y,\
		ih.bColorUsed,\
		ih.bColorImp);

#endif

	img.h = img.info_h.height;
	img.w = img.info_h.width;
	uint8_t padding = (4 - (img.w * 3) % 4) % 4;
	img.pixels = malloc(img.h * sizeof(pixel *));	
	for (int r = 0; r < img.h; r++) {
		img.pixels[r] = malloc(img.w * sizeof(pixel));
		fread(img.pixels[r], sizeof(pixel), img.w, fp);
		fseek(fp, padding, SEEK_CUR);
	}
	
	fclose(fp);
	errno = BMP_NO_ERROR;
	
	return &img;	
}

unsigned int bmp_save(bmp *img, const char *path) {
	FILE *fp = fopen(path, "wb");

	if (!fp)
		return FILE_ERROR;

	fwrite(&img->file_h, sizeof(bmp_file_h), 1, fp);
	fwrite(&img->info_h, sizeof(bmp_info_h), 1, fp);
	fseek(fp, img->file_h.offset, SEEK_SET);
	uint8_t padding = (4 - (img->w * 3) % 4) % 4;
	for (int r = 0; r < img->h; r++){
		fwrite(img->pixels[r], sizeof(pixel), img->w, fp);
		fseek(fp, padding, SEEK_CUR);
	}

	fclose(fp);
	return BMP_NO_ERROR;
}


imgObj* bmpToImgObj(bmp* img){
	imgObj* newImg = newImgObj(img->h, img->w);
	if (newImg == NULL){
		fprintf(stderr, "bmpToImgObj: Could not convert bmp to imgObj\n");
		return NULL;
	}

	for (size_t h = 0; h < (size_t) img->h; h++){
		for (size_t w = 0; w < (size_t) img->w; w++){
			uint8_t* pix = getPixel(newImg, h, w);
			pix[0] = img->pixels[h][w].r;
			pix[1] = img->pixels[h][w].g;
			pix[2] = img->pixels[h][w].b;
		}
	}
	return newImg;
}

bmp* imgObjToBmp(imgObj* img, bmp_file_h fileHeader, bmp_info_h infoHeader){

	if (img == NULL){
		fprintf(stderr, "imgObjToBmp: img is NULL\n");
		return NULL;
	}
	
	bmp* bmpImg = calloc(1, sizeof(bmp));
	if (bmpImg == NULL){
		fprintf(stderr, "imgObjToBmp: could not allocate memory\n");
		return NULL;
	}
	bmpImg->file_h = fileHeader;
	bmpImg->info_h = infoHeader;

	bmpImg->h = img->h;
	bmpImg->w = img->w;
	
	bmpImg->pixels = malloc(bmpImg->h * sizeof(pixel*));	
	for (int r = 0; r < bmpImg->h; r++) {
		bmpImg->pixels[r] = malloc(bmpImg->w * sizeof(pixel));
	}

	for (size_t h = 0; h < (size_t) img->h; h++){
                for (size_t w = 0; w < (size_t) img->w; w++){
                        uint8_t* pix = getPixel(img, h, w);
                        bmpImg->pixels[h][w].r = pix[0];
                        bmpImg->pixels[h][w].g = pix[1];
                        bmpImg->pixels[h][w].b = pix[2];
                }
        }
	return bmpImg;
}








