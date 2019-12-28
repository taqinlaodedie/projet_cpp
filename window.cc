#include "window.hh"
#include <time.h>
#include <stdlib.h>
#include "defines.hh"
#include <sstream>

using namespace std;

Window::Window(): win(nullptr), renderer(nullptr)
{
	quit = 0;
	mouseOnButton = false;
	record.open ("record/record.txt", std::ofstream::out | std::ofstream::app);
	mapInfo.open ("stage/level_barriers1.txt", std::ifstream::in);
	// Initialiser le SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	TTF_Init();

	// Creer une fenetre
	win= SDL_CreateWindow(WINDOW_CAPTION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, 0);
	// Initialiser la racine du random
	srand(unsigned (time(NULL)));
	// Charger les ressources
	loadResourceFile();
	// Charger le font
	for (int i=0; i<5; i++)
		fontTexs[i] = nullptr;
	font = TTF_OpenFont("res/sans.ttf", 20);
	init();
}

// Initialiser le jeu
void Window::init()
{
	game = new Game(vec, pTexs, bulletTex, blastTexs, barrierTexs);
	(*game) << mapInfo;
	creater = new EnemyCreater(*game, vec);
}

// Charger les ressources
void Window::loadResourceFile() 
{
	std::vector<SDL_Texture *> localVec;
	std::string buffer;
	// Enemi normal
	for(int i = 1; i <= 4; i++) {
		buffer = "res/Robot 1/robot1_gun";
		buffer += std::to_string(i);
		buffer += ".png";
		//std::cout << buffer << std::endl;
		localVec.push_back(loadImage(buffer.c_str()));
	}
	vec.push_back(localVec);

	localVec.clear();
	// Enemi fort
	for(int i = 1; i <= 4; i++) {
		buffer = "res/Zombie 1/zombie1_gun";
		buffer += std::to_string(i);
		buffer += ".png";
		//std::cout << buffer << std::endl;
		localVec.push_back(loadImage(buffer.c_str()));
	}
	vec.push_back(localVec);
	// Joueur
	for(int i = 1; i <= 4; i++) {
		buffer = "res/Hitman 1/hitman1_gun";
		buffer += std::to_string(i);
		buffer += ".png";
		//std::cout << buffer << std::endl;
		pTexs.push_back(loadImage(buffer.c_str()));
	}
	// Balle
	bulletTex = loadGifImage("res/bullet.gif");
	// Exposion
	for (int i = 1; i <= 3; i++)
	{
		buffer = "res/blast/blast";
		buffer += std::to_string(i);
		buffer += ".gif";
		blastTexs.push_back(loadGifImage(buffer));
	}
	blastTexs.push_back(loadGifImage("res/blast/blast3.gif"));
	// Obstacle
	barrierTexs.push_back(loadGifImage("res/barrier/normal.gif"));
	barrierTexs.push_back(loadGifImage("res/barrier/steel.gif"));
	barrierTexs.push_back(loadGifImage("res/barrier/grass.gif"));
	barrierTexs.push_back(loadGifImage("res/barrier/water.gif"));
	// Fond
	bg = loadImage("res/bg/1.png");
	cover = loadImage("res/cover.jpeg");
	smoke = loadImage("res/smoke.png");

	gameTexs[0] = loadImage("res/button1.png");
	gameTexs[1] = loadImage("res/button1_1.png");
	gameTexs[2] = loadImage("res/title.png");
}

Window::~Window() 
{
	TTF_CloseFont(font);
	TTF_Quit();

	// Supprimer les textures
	destroyTexture();

	if(game)
		delete game;

	mapInfo.close();
	record.close();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Window::start() 
{
	SDL_Event event;

	while(!quit) {
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:	// Clavier
					switch(event.key.keysym.sym) {
						case SDLK_w:
							game->handleInput(UP);
							break;
						case SDLK_a:
							game->handleInput(LEFT);
							break;
						case SDLK_d:
							game->handleInput(RIGHT);
							break;
						case SDLK_s:
							game->handleInput(DOWN);
							break;
						case SDLK_j:
							game->attack();
							break;
						case SDLK_ESCAPE:
							quit = true;
							record << *game << std::endl;	// Enregistrer le record en sortant le jeu
							break;
						case SDLK_SPACE:
							record << *game << std::endl;
							break;
						default:
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					if(!game->started) {
						// Boutton commencer
						if(event.button.x > START_X_POS && event.button.x < (START_X_POS + START_WIDTH) &&
							event.button.y > START_Y_POS && event.button.y < (START_Y_POS + START_HEIGHT)) {
							mouseOnButton = true;
						}
						else
							mouseOnButton = false;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT) {
						// Button commencer
						if(event.button.x > START_X_POS && event.button.x < (START_X_POS + START_WIDTH) &&
							event.button.y > START_Y_POS && event.button.y < (START_Y_POS + START_HEIGHT)) {
							game->newGame();
							game->started = true;
						}
					}
					break;
				default:
					break;
			}
		}
		update();
		SDL_RenderPresent(renderer);
	}
}

void Window::update()
{
	SDL_Delay(15);
	SDL_RenderClear(renderer);
	if(game->started) {
		creater->create();
		drawBackground();	// Afficher le fond
		drawGameInfo();	// Afficher les info du jeu
		drawSmoke();
		game->draw(renderer);
	}
	else {
		drawCover();
		drawTitle();
		drawButton();
	}
	if(game->gameOver())
		quit = true;
}

// Charger les images
SDL_Texture *Window::loadImage(std::string filePath)
{
	SDL_Surface *picture = nullptr;

	picture = IMG_Load(filePath.c_str());

	// Transparente
	Uint32 colorkey = SDL_MapRGB(picture->format, 255, 255, 255);
	SDL_SetColorKey(picture, SDL_TRUE, colorkey);

	SDL_Texture *res = SDL_CreateTextureFromSurface(renderer, picture);
	SDL_FreeSurface(picture);

	return res;
}

void Window::drawText(std::string message, int x, int y, int flag)
{
	SDL_Color color = { 0, 0, 0 };
	//std::cout << message.c_str() << std::endl;
	SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);

	if(fontTexs)
	{
		SDL_DestroyTexture(fontTexs[flag]);
		fontTexs[flag] = nullptr;
	}
	fontTexs[flag] = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int width = 0, height = 0;
	SDL_QueryTexture(fontTexs[flag], NULL, NULL, &width, &height);
	SDL_Rect rect = { x, y, width, height };
	SDL_RenderCopy(renderer, fontTexs[flag], NULL, &rect);
}

SDL_Texture* Window::loadGifImage(std::string filePath)
{
	SDL_Surface* pic = nullptr;
	SDL_Texture* tex = nullptr;

	SDL_RWops* rWops = SDL_RWFromFile(filePath.c_str(), "rb");
	pic = IMG_LoadGIF_RW(rWops);
	tex = SDL_CreateTextureFromSurface(renderer, pic);

	SDL_FreeSurface(pic);

	return tex;
}

void Window::drawBackground()
{
	// Le fond de l'ecran
	int width = 0, height = 0;
	SDL_QueryTexture(bg, NULL, NULL, &width, &height);
	SDL_Rect rect = { GAME_RIGHT_AREA, 0, width, height };
	SDL_RenderCopy(renderer, bg, NULL, &rect);
}

void Window::drawSmoke()
{
	int width = 0, height = 0;
	SDL_QueryTexture(smoke, NULL, NULL, &width, &height);
	SDL_Rect rect = { SMOKE_X_POSITION, SMOKE_Y_POSITION, width, height };
	SDL_RenderCopy(renderer, smoke, NULL, &rect);
}

void Window::drawCover()
{
	SDL_Rect rect = { 0, 0, GAME_RIGHT_AREA + 200, GAME_BOTTOM_AREA };
	SDL_RenderCopy(renderer, cover, NULL, &rect);
}

void Window::drawGameInfo()
{
	std::string score = "Score: ";
	score += std::to_string(game->getScore());
	drawText(score, SCORE_X_POSITION, SCORE_Y_POSITION, 0);

	std::string gameLevel = "Game level: ";
	gameLevel += std::to_string(game->getGameLevel());
	drawText(gameLevel, SCORE_X_POSITION, GAME_LEVEL_POSITION, 1);
}

void Window::drawTitle()
{
	int width = 0, height = 0;
	SDL_QueryTexture(gameTexs[2], NULL, NULL, &width, &height);
	SDL_Rect rect = { TITLE_X_POS, TITLE_Y_POS, width, height };
	SDL_RenderCopy(renderer, gameTexs[2], NULL, &rect);
}

void Window::drawButton()
{
	int width = 0, height = 0;

	if (!mouseOnButton)
	{
		SDL_QueryTexture(gameTexs[0], NULL, NULL, &width, &height);
		SDL_Rect rect = { START_X_POS, START_Y_POS, width, height };
		SDL_RenderCopy(renderer, gameTexs[0], NULL, &rect);
	}
	else
	{
		SDL_QueryTexture(gameTexs[1], NULL, NULL, &width, &height);
		SDL_Rect rect = { START_X_POS, START_Y_POS, width, height };
		SDL_RenderCopy(renderer, gameTexs[1], NULL, &rect);
	}
}

void Window::destroyTexture()
{
	for(int i=0; i<int(vec.size()); i++) {
		for(std::vector<SDL_Texture *>::const_iterator it = vec[i].cbegin(); it != vec[i].end(); it++) {
			if(*it)
				SDL_DestroyTexture(*it);
		}
	}
	vec.clear();

	for(std::vector<SDL_Texture *>::const_iterator it = pTexs.cbegin(); it != pTexs.cend(); it++) {
		if(*it)
			SDL_DestroyTexture(*it);
	}
	pTexs.clear();

	SDL_DestroyTexture(bulletTex);

	for(std::vector<SDL_Texture *>::const_iterator it = blastTexs.cbegin(); it != blastTexs.cend(); it++) {
		if(*it)
			SDL_DestroyTexture(*it);
	}
	blastTexs.clear();

	for(std::vector<SDL_Texture *>::const_iterator it = barrierTexs.cbegin(); it != barrierTexs.cend(); it++)
	{
		if(*it)
			SDL_DestroyTexture(*it);
	}

	SDL_DestroyTexture(bg);

	for (int i=0; i<5; ++i)
		SDL_DestroyTexture(fontTexs[i]);
	for (int i=0; i<5; ++i)
		SDL_DestroyTexture(gameTexs[i]);
}