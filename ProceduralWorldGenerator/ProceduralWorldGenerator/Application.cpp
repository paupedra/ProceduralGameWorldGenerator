#include "Application.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include "ProceduralGameWorldGeneratorLib/ProceduralGameWorldGenerator.h"

//#include "Dependencies/SDL/include/SDL3/SDL.h"

bool Application::Init()
{
	SDL_Init(SDL_INIT_VIDEO);

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
	
	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 10; ++y)
		{
			Entity* entity = new Entity(Position(x * 10, y * 10));

			entity->color.r = SDL_rand(255);
			entity->color.g = SDL_rand(255);
			entity->color.b = SDL_rand(255);
			
			entities.push_back((*entity));
		}
	}

	//SDL_Init(SDL_INIT_VIDEO);

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

	GenerateWorld();

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

}

void Application::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			exit = true;
		}
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			if (event.key.key == SDLK_RIGHT)
			{
				screenPos.x -= 100 * dt;
			}
			if (event.key.key == SDLK_LEFT)
			{
				screenPos.x += 100 * dt;
			}
			if (event.key.key == SDLK_UP)
			{
				screenPos.y += 100 * dt;
			}
			if (event.key.key == SDLK_DOWN)
			{
				screenPos.y -= 100 * dt;
			}
		}
	}
}

void Application::Render()
{
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i].Render(renderer, screenPos);
	}
}
