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
	
	img.h = img.info_h.height;
	img.w = img.info_h.width;

	img.pixels = malloc(img.h * sizeof(pixel *));	
	for (int r = 0; r < img.h; r++) {
		img.pixels[r] = malloc(img.w * sizeof(pixel));
		fread(img.pixels[r], sizeof(pixel), img.w, fp);
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

	for (int r = 0; r < img->h; r++)
		fwrite(img->pixels[r], sizeof(pixel), img->w, fp);

	fclose(fp);
	return BMP_NO_ERROR;
}
