#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

	void start(SDL_Surface*);
	void loadLvl(char(*)[NLBq],char);
	bool scanBrique(char(*)[NLBq]);
	void win(SDL_Surface*, bool);
	bool pause(SDL_Surface*, bool);
	bool moveBall(int*, char*, int*, char(*)[NLBq], int*);
	bool test(int, int);
	void moveBarre(int*, char);
	void blit(SDL_Surface*, int*, int*, int, char, char);
	void blitCard(SDL_Surface*, char(*)[NLBq], char);
	void testBq(int ,int ,char*, char(*)[NLBq], int*);

#endif
