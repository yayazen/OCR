void threshold_image(FILE* input_file, FILE* output_file, unsigned long vector_size, int threshold_value)
{
   unsigned char*   p_char;
   unsigned char    dummy;
   struct_img       binary_img;
   unsigned char*   binary_data;

   dummy = '0';
   p_char = &dummy;

   binary_img.data = malloc(vector_size * sizeof(char));
   
   if(binary_img.data == NULL)
     {
       printf("Failed to malloc binary_img.data\n");
     }

   binary_data = binary_img.data;

   /* CONVERT PIXEL TO BLACK AND WHITE BASED ON THRESHOLD VALUE */
   for(int i = 0; i < vector_size - 1; i++)
     {
       fread(p_char, sizeof(char), 1, input_file);
       if(*p_char < threshold_value) *(binary_data + i) = 0;
       else *(binary_data + i) = 255;
       fwrite((binary_data + i), sizeof(char), 1, output_file);
     }

   /* FREE ALLOCATED MEMORY */
   free(binary_data);
}
