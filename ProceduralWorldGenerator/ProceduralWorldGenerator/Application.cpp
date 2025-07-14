#include "Application.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include "ProceduralGameWorldGeneratorLib/ProceduralGameWorldGenerator.h"

//#include "Dependencies/SDL/include/SDL3/SDL.h"

bool Application::Init()
{
	window = SDL_CreateWindow("Hello World", 800, 600, SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		SDL_Log("Couldn't create window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		SDL_Log("Couldn't create renderer: %s", SDL_GetError());
		return false;
	}

	int w = 0, h = 0;
	float x, y;
	const float scale = 4.0f;

	/* Center the message and scale it up */
	SDL_GetRenderOutputSize(renderer, &w, &h);
	SDL_SetRenderScale(renderer, scale, scale);
	
	GenerateWorld(); //Call Library Here

	return true;
}

void Application::PrepareUpdate()
{
	currentFrameTimeStart = SDL_GetTicks();
	dt = lastFrameTime / 1000;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

bool Application::Update()
{
	PrepareUpdate();

	PollEvents();

	Render();

	FinishUpdate();

	return true;
}

void Application::FinishUpdate()
{
	if (SDL_GetTicks() - lastSecTimeStart > 1000)
	{
		lastSecTimeStart = SDL_GetTicks();
		prevLastSecFrames = lastSecFrames;
		lastSecFrames = 0;
	}

	float avg_fps = float(framesSinceStart) / (SDL_GetTicks() / 1000);
	float secondsSinceStart = (SDL_GetTicks() / 1000);

	//int frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "FPS: %.2f Av.FPS: %.2f Last Frame Ms: %f Last sec frames: %i  Time since startup: %.3f Frame Count: %lu Dt: %f",
		frameRate, avg_fps, lastFrameTime, prevLastSecFrames, secondsSinceStart, framesSinceStart, dt);

	SDL_SetWindowTitle(window, title);

	float frameTime = SDL_GetTicks() - currentFrameTimeStart;

	if (frameRate != 0)
	{
		if ((1000 / frameRate) > frameTime)
		{

			SDL_Delay((1000 / frameRate) - frameTime);

		}
	}

	lastFrameTime = SDL_GetTicks() - currentFrameTimeStart;

	framesSinceStart++;
	lastSecFrames++;

	SDL_RenderPresent(renderer);
}

bool Application::ShutDown()
{
	SDL_DestroyWindow(window);
	return true;
}

void Application::GenerateWorld()
{
	WorldInfoTopView2D topViewWorld;

	topViewWorld.width = 200;
	topViewWorld.height = 200;

	GenerateTowView2DWorld(topViewWorld, SDL_rand(10000000000));

	//Uint32 pixels[topViewWorld.width * topViewWorld.height];

	entities.clear();

	for (int y = 0; y < topViewWorld.width; ++y)
	{
		for (int x = 0; x < topViewWorld.height; ++x)
		{

			Entity* entity = new Entity(Position(x * tileSize , y * tileSize ));
			entity->size.x = tileSize;
			entity->size.y = tileSize;

			if (topViewWorld.tiles[x][y].tileId == 1)
			{
				entity->color = Color(0, 0, 255);
			}
			if (topViewWorld.tiles[x][y].tileId == 2)
			{
				entity->color = Color(0, 155, 255);
			}
			if (topViewWorld.tiles[x][y].tileId == 3)
			{
				entity->color = Color(211, 169, 108);
			}
			if (topViewWorld.tiles[x][y].tileId == 4)
			{
				entity->color = Color(0, 255, 0);
			}

			entities.push_back(*entity);
		}
		
	}
}

void Application::PollEvents()
{

	// Get the current keyboard state
	const bool* keyboardState = SDL_GetKeyboardState(NULL);

	// Check key states (for movement, actions, etc.)
	if (keyboardState[SDL_SCANCODE_RIGHT]) {
		cameraPosition.x -= cameraSpeed * dt;
	}
	if (keyboardState[SDL_SCANCODE_LEFT]) {
		cameraPosition.x += cameraSpeed * dt;
	}
	if (keyboardState[SDL_SCANCODE_UP]) {
		cameraPosition.y += cameraSpeed * dt;
	}
	if (keyboardState[SDL_SCANCODE_DOWN]) {
		cameraPosition.y -= cameraSpeed * dt;
	}
	if (keyboardState[SDL_SCANCODE_KP_8]) {
		zoom += zoomSpeed * dt;
	}
	if (keyboardState[SDL_SCANCODE_KP_2]) {
		zoom -= zoomSpeed * dt;
	}

	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_EVENT_QUIT) {
			exit = true;
		}
		if (event.type == SDL_EVENT_KEY_DOWN) {

			switch (event.key.key) {
			case SDLK_R:
				GenerateWorld();
				break;
			}
		}
	}
}

void Application::Render()
{
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i].Render(renderer, cameraPosition, zoom);
	}
}

void Application::GetCoordinateFromIndex(int index, int width, int* x, int* y)
{
	*x = index % width;
	*y = index / width;	
}
