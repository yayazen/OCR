#include "proc.h"
#include <stdint.h>

#define PI 3.141592


void fconv(GdkPixbuf *src, float dst[784]) {
	guchar* pixels = gdk_pixbuf_get_pixels(src);
    size_t rowstride = gdk_pixbuf_get_rowstride(src);
    size_t n_channels = gdk_pixbuf_get_n_channels(src);
    size_t height = gdk_pixbuf_get_height(src);
    size_t width = gdk_pixbuf_get_width(src);

    uint8_t *px;
    printf("h : %ld, w : %ld\n", height, width);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            
            px = pixels + i * rowstride + j * n_channels;
            dst[i * height + j] = (px[0] == 255) ? 0.0f : 1.0f;
        }
    }
}

int **kernel(int size) {
	int **k = malloc(size * sizeof(int *));
	for(int r = 0; r < size; r++) {
		k[r] = malloc(size * sizeof(int));
		memset(k[r], 1, size);
	}	
	return k;
}



void grayscale(GdkPixbuf* img){
	if (img == NULL){
		fprintf(stderr, "grayscale: NULL image\n");
		return;
	}

	uint8_t v = 0;
	
	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);
	
	for (size_t h = 0; h < pixbufHeight; h++){
		for (size_t w = 0; w < pixbufWidth; w++){
			uint8_t* pix = &pixels[h * rowstride + w * n_channels];
			v = (uint8_t) 0.2126 * pix[0] + 0.7152 * pix[1] + 0.0722 * pix[2];
			pix[0] = v;
			pix[1] = v;
			pix[2] = v;
		}
	}
}

double threshold(GdkPixbuf* img){
	if (img == NULL){
		fprintf(stderr, "threshold: NULL image\n");
		return NAN;
	}

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	double sum = 0;
	for (size_t h = 0; h < pixbufHeight; h++){
		for (size_t w = 0; w < pixbufWidth; w++){
			sum += pixels[h * rowstride + w * n_channels];
		}
	}
	return sum / (pixbufHeight * pixbufWidth);
}

void binarization(GdkPixbuf* img, double t){
	if (img == NULL){
		fprintf(stderr, "binarization: NULL imgae\n");
		return;
	}

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	for (size_t h = 0; h < pixbufHeight; h++){
		for (size_t w = 0; w < pixbufWidth; w++){
			uint8_t* pix = &pixels[h * rowstride + w * n_channels];
			uint8_t p = (pix[0] > t) ? 255 : 0;
			pix[0] = p;
			pix[1] = p;
			pix[2] = p;
		}
	}
}

GdkPixbuf* proximal_interpolation(GdkPixbuf* img, size_t height, size_t width){
	if (img == NULL){
		fprintf(stderr, "proximalInerpolation: img is NULL\n");
		return NULL;
	}
	
	GdkPixbuf* newImg = NULL;

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	if (!(pixbufHeight == height && pixbufWidth == width)){
	
		double imgAspectRatio = (double) pixbufWidth / (double) pixbufHeight ;
		double resultAspectRatio = (double) width / (double) height;
		double coordFact;		

		if (imgAspectRatio < resultAspectRatio ){
			// height will be the reference
			
			double coordRatio = (double) height / (double) pixbufHeight;
			coordFact = (double) pixbufHeight / (double) height;
			size_t newWidth = coordRatio * pixbufWidth;
			
			newImg = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, newWidth, height);
		
		} else {
			// width will be the reference

			double coordRatio = (double) width / (double) pixbufWidth;
			coordFact = (double) pixbufWidth / (double) width;
                        size_t newHeight = coordRatio * pixbufHeight;
                        
			newImg = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, newHeight);
		
		}
                if (newImg == NULL){
                        fprintf(stderr, "proxiamlInterpolation: could not create GdkPixbuf\n");
                        return NULL;
                }
                
		guchar* new_pixels = gdk_pixbuf_get_pixels(newImg);
        	size_t new_rowstride = gdk_pixbuf_get_rowstride(newImg);
        	size_t new_n_channels = gdk_pixbuf_get_n_channels(newImg);
        	size_t new_pixbufHeight = gdk_pixbuf_get_height(newImg);
        	size_t new_pixbufWidth = gdk_pixbuf_get_width(newImg);


		for (size_t h = 0; h < new_pixbufHeight; h++){
                        for (size_t w = 0; w < new_pixbufWidth; w++){
                        
                        	uint8_t* newPix = &new_pixels[h * new_rowstride + w * new_n_channels];
				size_t height = MIN(h * coordFact, pixbufHeight - 1);
				size_t width = MIN(w * coordFact, pixbufWidth - 1);
                                uint8_t* pix = &pixels[height * rowstride + width * n_channels];
                                if (pix == NULL){
					fprintf(stderr, "proximal_interpolation: could not access pixel\n");
				}
				
				newPix[0] = pix[0];
                                newPix[1] = pix[1];
                                newPix[2] = pix[2];

                        }
                }
		
	}
	
	return newImg;
}

GdkPixbuf* filling(GdkPixbuf* img, size_t height, size_t width){
	if (img == NULL){
		fprintf(stderr, "filling: img is NULL\n");
		return NULL;
	}

	guchar* pixels = gdk_pixbuf_get_pixels(img);
        size_t rowstride = gdk_pixbuf_get_rowstride(img);
        size_t n_channels = gdk_pixbuf_get_n_channels(img);
        size_t pixbufHeight = gdk_pixbuf_get_height(img);
        size_t pixbufWidth = gdk_pixbuf_get_width(img);

	if (pixbufHeight > height || pixbufWidth > width){
		fprintf(stderr, "filling: img is larger than target size\n");
		return NULL;
	}
	if (pixbufHeight == height && pixbufWidth == width){
		return img;
	}

	GdkPixbuf* newImg = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
	
	if (newImg == NULL){
		fprintf(stderr, "filling: could not create new image\n");
		return NULL;
	}


	size_t startHeight = (height - pixbufHeight) / 2;
	size_t startWidth = (width - pixbufWidth) / 2;
	size_t new_n_channels = gdk_pixbuf_get_n_channels(newImg);
	size_t new_rowstride = gdk_pixbuf_get_rowstride(newImg);
	guchar* new_pixels = gdk_pixbuf_get_pixels(newImg);

	for (size_t h = 0; h < pixbufHeight; h++){
		for (size_t w = 0; w < pixbufWidth; w++){
			uint8_t* pix = &pixels[h * rowstride + w * n_channels];
			uint8_t* newPix = &new_pixels[(startHeight + h) * new_rowstride + (startWidth + w) * new_n_channels];
		
			newPix[0] = pix[0];
			newPix[1] = pix[1];
			newPix[2] = pix[2];
		}
	}

	for (size_t h = 0; h < startHeight; h++){
		for (size_t w = 0; w < width; w++){
			uint8_t* pix = &new_pixels[h * new_rowstride + w * new_n_channels];
			pix[0] = 255;
			pix[1] = 255;
			pix[2] = 255;
		}
	}

	for (size_t h = pixbufHeight + startHeight; h < height; h++){
                for (size_t w = 0; w < width; w++){
			uint8_t* pix = &new_pixels[h * new_rowstride + w * new_n_channels];
                        pix[0] = 255;
                        pix[1] = 255;
                        pix[2] = 255;
                }
        }

	for (size_t w = 0; w < startWidth; w++){
                for (size_t h = 0; h < height; h++){
			uint8_t* pix = &new_pixels[h * new_rowstride + w * new_n_channels];
                        pix[0] = 255;
                        pix[1] = 255;
                        pix[2] = 255;
                }
        }

	for (size_t w = pixbufWidth + startWidth; w < width; w++){
                for (size_t h = 0; h < height; h++){
			uint8_t* pix = &new_pixels[h * new_rowstride + w * new_n_channels];
                        pix[0] = 255;
                        pix[1] = 255;
                        pix[2] = 255;
                }
        }

	return newImg;
}

GdkPixbuf* fit_image(GdkPixbuf* img, size_t height, size_t width, size_t border){
        if (img == NULL){
                fprintf(stderr, "fit_image: img is NULL\n");
                return NULL;
        }

        if (height == 0 || width == 0){
                fprintf(stderr, "fitImgae: invalid parameters\n");
                return NULL;
        }
        
	GdkPixbuf* scaledImg = proximal_interpolation(img, height - border, width - border);
	GdkPixbuf* fittedImg = filling(scaledImg, height, width);
	if (scaledImg != fittedImg){
		g_object_unref(scaledImg);
	}

	return fittedImg;
}



double hough(size_t* points, size_t len, double precision){
	size_t angles = (size_t) 360/precision;
	double* avrg = calloc(angles, sizeof(double));
	if (avrg == NULL) {
		errno = 1001;
		fprintf(stderr, "Hough: could not allocate memory\n");
		return NAN;
	}
	double* k = calloc(angles * len, sizeof(double));
	if (k == NULL) {
                errno = 1002;
                fprintf(stderr, "Hough: could not allocate memory\n");
		free(avrg);
                return NAN;
        }
	double* range = calloc(angles, sizeof(double));
	if (range == NULL) {
                errno = 1003;
                fprintf(stderr, "Hough: could not allocate memory\n");
		free(k);
		free(avrg);
                return NAN;
        }
	size_t minRange = angles;
	
	for (size_t i = 0; i < len; i++){
		for (size_t teta = 0; teta < angles; teta++){
			k[i * angles + teta] = sqrt( points[2*i] * points[2*i] + points[2*i+1] + points[2*i+1])\
				* sin(teta * precision * PI / 180.0);
			avrg[teta] += k[i * angles + teta];
		}
	}
	
	for (size_t teta = 0; teta < angles; teta++){
		avrg[teta] /= angles;
		for (size_t i = 0; i < len; i++){
			double v = k[i * angles + teta] - avrg[teta];
			range[teta] += (v >= 0)? v : -v;
		}
		if ((minRange == angles) || (range[teta] < range[minRange]))
			minRange = teta;
	}
	free(avrg);
	free(k);
	free(range);
	return minRange * precision;
}


//TODO
size_t* findPointsOfInterest(GdkPixbuf* img, size_t n){
	if (img == NULL){
		fprintf(stderr,"findPointsOfInterest: GdkPixbuf is NULL, could not proceed\n");
		return NULL;
	}
	size_t* points = calloc(2 * n, sizeof(size_t));
	if (points == NULL) {
                errno = 2001;
                fprintf(stderr, "findPointsOfInterest: could not allocate memory\n");
		return NULL;
        }
	

	fprintf(stderr, "findPointsOdInterest: Unimplemented Function");
	errno = 2000;
	return NULL;
	
}

/*
void findAngle(bmp* img) {
	
	for (size_t y = 0; y < img.h; y++) {
		for (size_t x = 0; x < img.w; x++) {
			
		}
	}

}*/
