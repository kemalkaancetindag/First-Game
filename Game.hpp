#pragma once
#ifndef  Game_hpp
#define Game_hpp
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "AssetManager.h"

class ColliderComponent;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static AssetManager* assets;
	
	static bool isRunning;

	static SDL_Rect camera;
	enum groupLabels : std::size_t {
		groupMap,
		groupPalyers,		
		groupColliders
	};





private:
	
	SDL_Window* window;	
};

#endif // ! Game_hpp
