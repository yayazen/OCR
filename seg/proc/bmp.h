#ifndef BMP_H
#define	BMP_H

#define BMP_NO_ERROR	0
#define	INVALID_FORMAT	1
#define	FILE_ERROR	2

#include "img.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#pragma pack(push, 1)
struct bmpFileHeader
{
        uint16_t        type;
        uint32_t        size;

	uint32_t	reserved;	

        uint32_t        offset;

};
typedef struct bmpFileHeader bmp_file_h;
#pragma pack(pop)

#pragma pack(push,1)
struct bmpInfoHeader
{
        uint32_t        bstruct_size;
        uint32_t        width;
        uint32_t        height;

        uint16_t        bplanes;
        uint16_t        bitcount;

        uint32_t        bcompression;
        uint32_t        img_size;

        uint32_t        Pxmeter_x;
        uint32_t        Pxmeter_y;
        uint32_t        bColorUsed;
        uint32_t        bColorImp;

};
typedef struct bmpInfoHeader bmp_info_h;
#pragma pack(pop)

#pragma pack(push, 1)
struct Pixel
{	
	uint8_t		b, g, r;
};
typedef struct Pixel pixel;
#pragma pack(pop)


struct BMP {
	bmp_file_h	file_h;
	bmp_info_h	info_h;
	
	int 		h, w;
	pixel		**pixels;
};
typedef struct BMP bmp;

size_t fpread(void *buf, size_t size, size_t offset, FILE *fp);

void bmp_close(bmp *img);

bmp * bmp_load(const char *path);

unsigned int bmp_save(bmp *img, const char *path);

imgObj* bmpToImgObj(bmp* img);

bmp* imgObjToBmp(imgObj* img, bmp_file_h fileHeader, bmp_info_h infoHeader);

#endif
