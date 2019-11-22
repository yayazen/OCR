#include <stdlib.h>

#include <stdio.h>

#include <SDL/SDL.h>


//Displaying an image
SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


//Waiting for a key to be pressed
void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);

































void lines_splitting(SDL_Surface* image_surface, int width, int height)
{
    Uint8 r, g, b;
    Uint32 pixel;
    int bool =0;
    int coordinates_x;
    int array_nbr=0;
      
      int array[];

      int new_lines  =0;

      
      while(y<height)
      {
          while(x<width )
        {
            pixel = get_pixel(image_surface, x, y);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            
            if(r+g+b == 765) //start of a character
            {
                x=0;

                if(new_lines ==0)
                {
                    new_lines =1;
                }
                
            }
            else
            {
                if(new_lines == 1) //meaning we are searching a lines
                {
                    array

                }
                x++;
            }



            //ignore a white line

            
        }
          y++;
    }

      display_image(SDL_Surface *img);

          wait_for_keypressed();

      
    
}
