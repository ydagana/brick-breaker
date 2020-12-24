#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "functions.h"

int main(int argc, char** argv)
{
    SDL_Surface *screen = NULL, *starting = NULL;
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_WM_SetCaption("Casse Brique", NULL);
    SDL_WM_SetIcon(IMG_Load("images/ballon.gif"), NULL);
    screen = SDL_SetVideoMode(L, H + 20, 32, SDL_SWSURFACE|SDL_DOUBLEBUF);
    starting = IMG_Load("images/starting.jpg");
    SDL_BlitSurface(starting, NULL, screen, &pos);
    bool cont = true;
    SDL_Event event;
	SDL_EnableKeyRepeat(20, 20);
    while(cont)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                 cont = false;
                 break;
            case SDL_KEYDOWN:
				 switch(event.key.keysym.sym)
				 {
				 	case SDLK_ESCAPE:
						 cont = false;
						 break;
					case SDLK_RETURN:
						 start(screen);
						 cont = false;
						 break;
				 }
				 break;
        }
        SDL_Flip(screen);
    }
    SDL_FreeSurface(screen);
    SDL_FreeSurface(starting);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
