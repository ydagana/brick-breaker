#include <iostream>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "functions.h"

using namespace std;

void start(SDL_Surface* screen)
{
	char lives = 3, lvl = 1;
	int t0 = 0, t = 0, score = 0;
	bool bq = true, close = false;
	char carte[NHBq][NLBq];
	loadLvl(carte, lvl);
	while((lives >= 0)&&(!close))
	{
		int x = 0;
		srand(time(NULL));
		x = (rand()%(400));
		bool cont = true, save = true;
		int ball[2] = {x + (LBr - DB)/2, H - DB}, barre = x;
		char dir = x%2;
		SDL_Event event;
		while((cont)&&(!close))
		{
			bq = scanBrique(carte);
			if(!bq)
			{
				lvl++;
				if(lvl > Nlvl)
				{
					win(screen, true);
					close = pause(screen, false);
					close = true;
				}
				else
				{
					x = (rand()%(400));
					bq = true;
					loadLvl(carte, lvl);
					ball[1] = x + (LBr - DB)/2;
					ball[2] = H - DB;
					barre = x;
					close = pause(screen, false);
				}
			}
			SDL_PollEvent(&event);
			t = SDL_GetTicks();
			if(t - t0 > 30 - score/10)
			{
				save = moveBall(ball, &dir, &barre, carte, &score);
				t0 = t;
			}
			else
				SDL_Delay(30 - t + t0);
			switch(event.type)
			{
				case SDL_QUIT:
					 close = true;
					 break;
				case SDL_KEYDOWN:
					 switch(event.key.keysym.sym)
					 {
						case SDLK_ESCAPE:
							 close = true;
							 break;
						case SDLK_SPACE:
							 close = pause(screen, true);
							 break;
						case SDLK_RIGHT:
							 moveBarre(&barre, R1);
							 break;
						case SDLK_LEFT:
						     moveBarre(&barre, L1);
							 break;
					 }
					 break;
			}
			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
			blitCard(screen, carte, lvl);
			blit(screen, ball, &barre, score, lives, lvl);
			if(!save)
			{
				lives--;
				cont = true;
				x = (rand()%(400));
				ball[1] = x + (LBr - DB)/2;
				ball[2] = H - DB;
				close = pause(screen, false);
				barre = x;
			}
			SDL_Flip(screen);
		}
	}
	if(lives < 0)
	{
		win(screen, false);
		pause(screen, false);
		SDL_Flip(screen);
	}
}

bool moveBall(int* ball, char* dir, int* barre, char carte[][NLBq], int *score)
{
	bool live = true;
	if(*dir == D1)
	{
		if(ball[X] >= L - DB)
			*dir = R1;
		else if(ball[Y] <= 0)
			*dir = U2;
		else
		{
			ball[X]+=A;
			ball[Y]-=A;
		}
	}
	if(*dir == R1)
	{
		if(ball[X] <= 0)
			*dir = L2;
		else if(ball[Y] <= 0)
			*dir = U1;
		else
		{
			ball[X]-=A;
			ball[Y]-=A;
		}
	}
	if(*dir == U1)
	{
		if(ball[X] <= 0)
			*dir = L1;
		else if(ball[Y] >= H - HBr - DB)
		{
			live = test(*barre, ball[X]);
			*dir = D2;
		}
		else
		{
			ball[X]-=A;
			ball[Y]+=A;
		}
	}
	if(*dir == L1)
	{
		if(ball[X] >= L - DB)
			*dir = R2;
		else if(ball[Y] >= H - HBr - DB)
		{
			live = test(*barre, ball[X]);
			*dir = D1;
		}
		else
		{
			ball[X]+=A;
			ball[Y]+=A;
		}
	}
	if(*dir == D2)
	{
		if(ball[X] <= 0)
			*dir = L2;
		else if(ball[Y] <= 0)
			*dir = U1;
		else
		{
			ball[X]-=A;
			ball[Y]-=A;
		}
	}
	if(*dir == R2)
	{
		if(ball[X] <= 0)
			*dir = L1;
		else if(ball[Y] >= H - HBr - DB)
		{
			live = test(*barre, ball[X]);
			*dir = D2;
		}
		else
		{
			ball[X]-=A;
			ball[Y]+=A;
		}
	}
	if(*dir == U2)
	{
		if(ball[X] >= L - DB)
			*dir = R2;
		else if(ball[Y] >= H - HBr - DB)
		{
			live = test(*barre, ball[X]);
			*dir = D1;
		}
		else
		{
			ball[X]+=A;
			ball[Y]+=A;
		}
	}
	if(*dir == L2)
	{
		if(ball[X] >= L - DB)
			*dir = R1;
		else if(ball[Y] <= 0)
			*dir = U2;
		else
		{
			ball[X]+=A;
			ball[Y]-=A;
		}
	}
	if(ball[X] > L - DB)
		ball[X] = L - DB;
	if(ball[X] < 0)
		ball[X] = 0;
	if(ball[Y] > H - HBr - DB)
		ball[Y] = H - HBr - DB;
	if(ball[Y] < 0)
		ball[Y] = 0;
	if((ball[Y] < 100)&&(ball[Y] > 19 ))
    {
        if((*dir == D1)||(*dir == L2)||(*dir == D2)||(*dir == R1))
            testBq(ball[X], ball[Y], dir, carte, score);
    }
    if((ball[Y] + DB < 100)&&(ball[Y] + DB > 19 ))
    {
        if((*dir == U1)||(*dir == R2)||(*dir == U2)||(*dir == L1))
            testBq(ball[X], ball[Y], dir, carte, score);
    }
	return live;
}

bool test(int x, int ballX)
{
	if((ballX >= x - 3*DB/4)&&(ballX <= x + LBr - DB/4))
		return true;
	else
		return false;
}

void testBq(int x, int y, char *dir, char carte[][NLBq], int* score)
{
	char y1 = (y - 19)/9, y2 = (y - 19 + DB/2)/9, y3 = (y - 19 + DB)/9;
	if(y1 < 0)
		y1 = 0;
	if(y2 < 0)
		y2 = 0;
	if(y2 >= NHBq)
		y2 = NHBq - 1;
	if(y3 >= NHBq)
		y3 = NHBq - 1;
	if((*dir == D1)||(*dir == L2))
	{
		if(carte[y1][(x+DB/2)/22] > 0)
		{
			carte[y1][(x+DB/2)/22]--;
			*score+=1;
			*dir = U2;
		}
		else if((carte[y1][x/22] <= 0)&&(carte[y2][(x+DB)/22] > 0))
		{
			carte[y2][(x+DB)/22]--;
			*score+=1;
			*dir = R1;
		}
	}
	else if((*dir == D2)||(*dir == R1))
	{
		if(carte[y1][(x+DB/2)/22] > 0)
		{
			carte[y1][(x+DB/2)/22]--;
			*score+=1;
			*dir = U1;
		}
		else if((carte[y2][x/22] > 0)&&(carte[y1][(x+DB)/22] <= 0))
		{
			carte[y2][x/22]--;
			*score+=1;
			*dir = L2;
		}
	}
	else if((*dir == R2)||(*dir == U1))
	{
		if(carte[y3][(x+DB/2)/22] > 0)
		{
			carte[y3][(x+DB/2)/22]--;
			*score+=1;
			*dir = D2;
		}
		else if((carte[y2][x/22] > 0)&&(carte[y3][(x+DB/2)/22] <= 0))
		{
			carte[y2][x/22]--;
			*score+=1;
			*dir = L1;
		}
	}
	else if((*dir == U2)||(*dir == L1))
	{
		if(carte[y3][(x+DB/2)/22] > 0)
		{
			carte[y3][(x+DB/2)/22]--;
			*score+=1;
			*dir = D1;
		}
		else if((carte[y3][x/22] <= 0)&&(carte[y2][(x+DB)/22] > 0))
		{
			carte[y2][(x+DB)/22]--;
			*score+=1;
			*dir = R2;
		}
	}
}

void moveBarre(int* barre, char dir)
{
	if(dir == L1)
	{
		*barre-=4;
		if(*barre < 0)
			*barre = 0;
	}
	else if(dir == R1)
	{
		*barre+=4;
		if(*barre > L - LBr)
			*barre = L - LBr;
	}
}

void loadLvl(char carte[][NLBq], char lvl)
{
	for(char i = 0; i < NHBq; i++)
	{
		for(char j = 0; j < NLBq; j++)
			carte[i][j] = 0;
	}
	FILE *level = NULL;
	char tab[22];
	if(lvl == 1)
		level = fopen("levels/level1.txt", "r");
	else if(lvl == 2)
		level = fopen("levels/level2.txt", "r");
	else if(lvl == 3)
		level = fopen("levels/level3.txt", "r");
	rewind(level);
	for(char i = 0; i < NHBq; i++)
	{
			fgets(tab, NLBq + 2, level);
			for(char j = 0; j < NLBq; j++)
			{
					if(tab[j] == '0')
						carte[i][j] = 0;
					else if(tab[j] == '1')
						carte[i][j] = 1;
					else if(tab[j] == '2')
						carte[i][j] = 2;
					else if(tab[j] == '3')
						carte[i][j] = 3;
			}
	}
	fclose(level);
}

void blitCard(SDL_Surface* screen, char carte[][NLBq], char lvl)
{
	SDL_Surface *b1 = NULL, *b2 = NULL, *b3 = NULL;
	SDL_Rect pos;
	b1 = IMG_Load("images/bB.jpg");
	b2 = IMG_Load("images/bR.jpg");
	b3 = IMG_Load("images/bY.jpg");
	for(char i = 0; i < 9; i++)
	{
		for(char j = 0; j < NLBq; j++)
		{
			if(carte[i][j] > 0)
			{
				pos.y = 19 + i*HBq;
				pos.x = j*LBq;
				if(carte[i][j] == 3)
					SDL_BlitSurface(b1, NULL, screen, &pos);
				else if(carte[i][j] == 2)
					SDL_BlitSurface(b2, NULL, screen, &pos);
				else if(carte[i][j] == 1)
					SDL_BlitSurface(b3, NULL, screen, &pos);
			}
		}
	}
    SDL_FreeSurface(b1);
    SDL_FreeSurface(b2);
    SDL_FreeSurface(b3);
}

bool scanBrique(char carte[][NLBq])
{
	for(char i = 0; i < NHBq; i++)
	{
		for(char j = 0; j < NLBq; j++)
		{
			if(carte[i][j] > 0)
				return true;
			else
				carte[i][j] = 0;
		}
	}
	return false;
}

void win(SDL_Surface* screen, bool cas)
{
	bool cont = true;
	SDL_Surface *txt = NULL;
	SDL_Rect pos;
	TTF_Font *police = NULL;
	police = TTF_OpenFont("fonts/angelina.ttf", 50);
	SDL_Color red = {255, 0, 0};
	SDL_Color bleu = {0, 0, 255};
	if(!cas)
		txt = TTF_RenderText_Blended(police, "Vous avez perdu", red);
	else
		txt = TTF_RenderText_Blended(police, "Bravo!, vous avez atteint la fin!!", red);
	pos.x = L/2 - txt->w/2;
	pos.y = H/2 - txt->h/2 - 30;
	SDL_BlitSurface(txt, NULL, screen, &pos);
	police = TTF_OpenFont("fonts/PLBOUP.ttf", 20);
	SDL_FreeSurface(txt);
	TTF_CloseFont(police);
}

void blit(SDL_Surface* screen, int* ball, int* barre, int score, char lives, char lvl)
{
	SDL_Surface *img = NULL;
	SDL_Rect pos;
	img = IMG_Load("images/line.jpg");
	pos.x = 0;
	pos.y = H;
	SDL_BlitSurface(img, NULL, screen, &pos);
	img = IMG_Load("images/barre.jpg");
	pos.x = *barre;
	pos.y = H - HBr;
	SDL_BlitSurface(img, NULL, screen, &pos);
	img = IMG_Load("images/ball.jpg");
	pos.x = ball[X];
	pos.y = ball[Y];
	SDL_BlitSurface(img, NULL, screen, &pos);
	img = IMG_Load("images/live.jpg");
	pos.x = 16;
	pos.y = H + 3;
	SDL_BlitSurface(img, NULL, screen, &pos);
	TTF_Font *police = NULL;
	police = TTF_OpenFont("fonts/PLBOUP.ttf", 12);
	SDL_Color bleu = {0, 64, 128};
	char chaine[40];
	sprintf(chaine, ": %d          score : %d            level : %d", lives, score, lvl);
	img = TTF_RenderText_Blended(police, chaine, bleu);
	pos.x = 46;
	pos.y = H + 3;
	sprintf(chaine, "jens++ production 2008");
	SDL_BlitSurface(img, NULL, screen, &pos);
	img = TTF_RenderText_Blended(police, chaine, bleu);
	pos.x = L - img->w;
	pos.y = H + 3;
	SDL_BlitSurface(img, NULL, screen, &pos);
	SDL_FreeSurface(img);
	TTF_CloseFont(police);
}

bool pause(SDL_Surface* screen, bool cas)
{
	bool cont = true;
	SDL_Event event;
	SDL_Surface *Pause = NULL;
	SDL_Rect pos;
	TTF_Font *police = NULL;
	police = TTF_OpenFont("fonts/PLBOUP.ttf", 20);
	SDL_Color color = {0, 0, 255};
	Pause = TTF_RenderText_Blended(police, "Appuyer sur Entrer pour continer", color);
	pos.x = L/2 - Pause->w/2;
	pos.y = H/2 - Pause->h/2 + 50;
	SDL_BlitSurface(Pause, NULL, screen, &pos);
	if(cas)
	{
		police = TTF_OpenFont("fonts/angelina.ttf", 50);
		Pause = TTF_RenderText_Blended(police, "Pause", color);
		pos.x = L/2 - Pause->w/2;
		pos.y = H/2 - Pause->h/2 - 20;
		SDL_BlitSurface(Pause, NULL, screen, &pos);
	}
	while(cont)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
                 cont = false;
                 return true;
                 break;
			case SDL_KEYDOWN:
			     switch(event.key.keysym.sym)
			     {
			     	case SDLK_ESCAPE:
						 cont = false;
						 return true;
						 break;
					case SDLK_RETURN:
						 cont = false;
						 return false;
						 break;
			     }
			     break;
		}
		SDL_Flip(screen);
	}
	SDL_FreeSurface(Pause);
	TTF_CloseFont(police);
}
