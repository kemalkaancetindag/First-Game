#include "Game.hpp"
#include "TextureManager.h"
#include "ECS//Vector2D.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Collision.h"


SDL_Event Game::event;
SDL_Renderer* Game::renderer = nullptr;
Map* map;

SDL_Rect Game::camera = { 0,0, 800, 640 };


Manager manager;
auto& player(manager.addEntity());

AssetManager* Game::assets = new AssetManager(&manager);


bool Game::isRunning = false;



Game::Game() {

}

Game::~Game() {

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsytems Initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (window) {
			std::cout << "Window Created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created" << std::endl;
		}

		isRunning = true;

	}
	else {
		isRunning = false;
	}

	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/player_anims.png");

	
	map = new Map("terrain", 3, 32);

	map->LoadMap("assets/map.map", 25, 20);



	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");

	player.addGroup(groupPalyers);


	

}


auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPalyers));
auto& colliders(manager.getGroup(Game::groupColliders));


void Game::update() {

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	
	manager.refresh();
	manager.update();

	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;
	

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
	
}



void Game::render() {
	SDL_RenderClear(renderer);	
	for (auto& t : tiles) {
		t->draw();
	}

	for (auto& c : colliders) {
		c->draw();
	}

	for (auto& p : players) {
		p->draw();
	}
	


	SDL_RenderPresent(renderer);
}


void Game::handleEvents() {
	
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}


void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
