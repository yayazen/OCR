#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


/*Load the image named my_image.jpg */
SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


void SDL_FreeSurface(SDL_Surface *surface);


int main()
{
    SDL_Surface* image_surface;

    init_sdl();

    image_surface = load_image("my_image.jpg");

    /* get the pixel value/ get tge rgb value/ compute the weighted average of the RGB values/ get the new pixel /put the new pixel */

	int width = image_surface->w;
	int height = image_surface->h;

	for (int y = 0; y<width; y++)
	{
		for (int x = 0; x <height;x++)
		{
			Uint32 pixel = get_pixel(image_surface, x, y);

			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

			Uint8 average  = 0.3*r + 0.59*g + 0.11*b;

			Uint32 new_pixel = SDL_MapRGB(image_surface->format, average, average, average);

			put_pixel(image_surface, x, y, new_pixel);
		}
	}
    /* Save the grayscale on the file my_new_image.jpg */

	SDL_SaveBMP(image_surface, "my_new_image.jpg");


    SDL_FreeSurface(image_surface);

    return 0;
}
