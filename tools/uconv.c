#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <gtk/gtk.h>


void tree(char *parent, int depth, char *label);

FILE *ifp;
FILE *lfp;

int main(int argc, char **argv)
{
    if (argc != 2) {
        return -1;
    }
    
    int flag_img = 0x03080000;
    int flag_label = 0x01080000;

    int dim = 0x1c000000;

   // int count = 0x10f60000; // 62992 all
     int count = 0x60ce0000; //52832 letters
//    int count = 0xb0270000; 10160 digit

    ifp = fopen("img-ubyte", "wb");
    lfp = fopen("label-ubyte", "wb");

    fwrite(&flag_img, 1, 4, ifp);
    fwrite(&flag_label, 1, 4, lfp);

    fwrite(&count, 1, 4, ifp);
    fwrite(&count, 1, 4, lfp);
    
    fwrite(&dim, 1, 4, ifp);
    fwrite(&dim, 1, 4, ifp);

    tree(argv[1], 1, NULL);
    

    fclose(ifp);
    fclose(lfp);
    
    return 0;
}



unsigned char write_data[28 * 28];

void build(GdkPixbuf *buf) {
    guchar *pixels = gdk_pixbuf_get_pixels (buf);
    uint8_t *p;

    int n_chan = gdk_pixbuf_get_n_channels (buf);
    int rs = gdk_pixbuf_get_rowstride(buf);


    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            p = pixels + i * rs + j * n_chan;
            
            write_data[i * 28 + j] = (p[0] > 100) ? 0 : 1;
        }
    }

}

int count = 0;

void tree(char *parent, int depth, char *label)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(parent);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {

            strcpy(path, parent);
            strcat(path, "/");
            strcat(path, dp->d_name);
            
            if (depth == 0) {
                build(gdk_pixbuf_new_from_file_at_size (path, 28, 28, NULL));
                fwrite(write_data, 1, 28*28, ifp);

                char l = (char) atoi(label);
                fwrite(&l, 1, 1, lfp);
                count++;
                printf("\b\b\b\b\b\b\b\b\b %d..",count);
            }
            
            tree(path, depth - 1, dp->d_name);
        }
    }

    closedir(dir);
}
