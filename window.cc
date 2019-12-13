#include "window.hh"

using namespace std;

Window::Window() {
	quit = 0;
}

Window::~Window() {
	cout << "Finish" << endl;
}

void Window::start() {
	// Initialiser le SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	// Creer une fenetre
	SDL_Window *window = SDL_CreateWindow("Smoke", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024+128, 720, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Charger les images
	coverImage = IMG_Load("img/cover.jpeg");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, coverImage);

	while(!quit) {
		SDL_Rect dstrect = { 0, 0, 1024, 720 };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		if (SDL_PollEvent(&event))
		{
			//printf("un event\n");
        	switch (event.type)
        	{
            	case SDL_QUIT:
                	quit = 1;
                	break;
				case  SDL_MOUSEBUTTONDOWN:
					cout << "click" << endl;
					break;
        	}
		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
    SDL_FreeSurface(coverImage);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}