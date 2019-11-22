/* INITIALIZE ARRAYS */
for(int i = 0; i < 255; i++) occurrence[i] = 0;
for(int i = 0; i < 255; i++) histogram[i] = 0;

/* START AT BEGINNING OF RASTER DATA */
fseek(input_img, (54 + 4 * color_number), SEEK_SET);

/* READ RASTER DATA */
for(int r = 0; r <= original_img.rows - 1; r++) {
    for(int c = 0; c <= original_img.cols -1; c++) {
        fread(p_char, sizeof(char), 1, input_img);
        pixel_value = *p_char;
        /* COUNT OCCURRENCES OF PIXEL VALUE */
        occurrence[pixel_value] = occurrence[pixel_value] + 1;
        total_pixels++;
    }
}

for(int i = 0; i <= 255; i++) {
    /* TAKES NUMBER OF OCCURRENCES OF A PARTICULAR PIXEL 
    * AND DIVIDES BY THE TOTAL NUMBER OF PIXELS YIELDING 
    * A RATIO */
    histogram[i] = (float) occurrence[i] / (float) total_pixels;
}

threshold_value = otsu_method(histogram, total_pixels);

threshold_image(//input file, //output file, //vector size?// threshold value);
