#include "Application.h"
#include <iostream>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "ProceduralGameWorldGeneratorLib/ProceduralGameWorldGenerator.h"

//#include "Dependencies/SDL/include/SDL3/SDL.h"

bool Application::Init()
{
	//(AGUNSEGUIR_MOLDS_DINERS)

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

	if (TTF_Init() == -1) {
		printf("TTF_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	InitUISliders(renderer);
	
	width = WIDTH;
	height = HEIGHT;
	waterPercent = 40.f;
	generatorZoom = 2.f;

	GenerateWorld(); //Call Library Here

	return true;
}

void Application::PrepareUpdate()
{
	currentFrameTimeStart = (int)SDL_GetTicks();
	dt = lastFrameTime / 1000.f;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	/*Uint32 buttons = SDL_GetMouseState(NULL, NULL);
	if (buttons & SDL_BUTTON_LMASK) {
		
	}*/

	for (int i = 0; i < sliders.size(); ++i)
	{
		sliders[i].Click(mouseX, mouseY,false);
	}
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
		lastSecTimeStart = (float)SDL_GetTicks();
		prevLastSecFrames = lastSecFrames;
		lastSecFrames = 0;
	}

	float avg_fps = float(framesSinceStart) / (SDL_GetTicks() / 1000);
	float secondsSinceStart = (float)(SDL_GetTicks() / 1000);

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
}

bool Application::ShutDown()
{
	SDL_DestroyWindow(window);
	return true;
}

void Application::InitUISliders(SDL_Renderer* renderer)
{
	int windowW, windowH;
	SDL_GetWindowSize(window, &windowW, &windowH);

	UISlider* zoomSlider = new UISlider(&zoom);
	zoomSlider->valueIncrements = 0.1;
	zoomSlider->minValue = 1.f;
	zoomSlider->maxValue = 10.f;
	zoomSlider->string = "ZOOM";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&width);
	zoomSlider->valueIncrements = 5;
	zoomSlider->minValue = 1.f;
	zoomSlider->maxValue = 10000.f;
	zoomSlider->string = "WIDTH";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&height);
	zoomSlider->valueIncrements = 5;
	zoomSlider->minValue = 1.f;
	zoomSlider->maxValue = 10000.f;
	zoomSlider->string = "HEIGHT";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&waterPercent);
	zoomSlider->valueIncrements = 1;
	zoomSlider->minValue = 0.f;
	zoomSlider->maxValue = 99.f;
	zoomSlider->string = "WATER%";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&generatorZoom);
	zoomSlider->valueIncrements = 0.1f;
	zoomSlider->minValue = 1.1f;
	zoomSlider->maxValue = 100.f;
	zoomSlider->string = "GENZOOM";
	sliders.push_back(*zoomSlider);

	for (int i = 0; i < sliders.size(); ++i)
	{
		sliders[i].Init(renderer);
	}
}

void Application::GenerateWorld()
{
	WorldInfoTopView2D topViewWorld;
	topViewWorld.tiles = nullptr;
	topViewWorld.biomeCount = 0;
	topViewWorld.width = width;
	topViewWorld.height = height;
	topViewWorld.assureWaterPercentage = false;
	topViewWorld.waterPercent = waterPercent;
	topViewWorld.zoom = generatorZoom;
	topViewWorld.addBeach = true;
	topViewWorld.beachPercent = 20;

	GenerateTopView2DWorld(&topViewWorld, SDL_rand(10000000000));

	entities.clear();

	// Create some pixel data (RGBA 32-bit format)
	std::vector<Uint32> pixels(topViewWorld.width * topViewWorld.height);

	Color color;
	for (int y = 0; y < topViewWorld.height; ++y)
	{
		for (int x = 0; x < topViewWorld.width; ++x)
		{

			//Entity* entity = new Entity(Position(x * tileSize , y * tileSize ));
			//entity->size.x = tileSize;
			//entity->size.y = tileSize;

			if (topViewWorld.tiles[y][x].tileId == 1)//Land
			{
				color = Color(0, 255, 0, 255);
			}
			if (topViewWorld.tiles[y][x].tileId == 2)//Water
			{
				color = Color(0, 0, 255, 255);
			}
			if (topViewWorld.tiles[y][x].tileId == 3)//Water
			{
				color = Color(255, 255, 0, 255);
			}

			pixels[y * topViewWorld.width + x] = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;

			//entities.push_back(*entity);
		}
		
	}

	dst = new SDL_FRect();
	dst->x = 0;
	dst->y = 0;
	dst->w = topViewWorld.width;
	dst->h = topViewWorld.height;

	mapTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		topViewWorld.width, topViewWorld.height);

	if (!mapTexture) {
		SDL_Log("Texture creation failed: %s", SDL_GetError());
	}
	else {
		// Update texture with our pixel data
		if (SDL_UpdateTexture(mapTexture, nullptr, pixels.data(), topViewWorld.width * sizeof(Uint32)) != 0) {
			SDL_Log("Texture update failed: %s", SDL_GetError());
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

		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			exit = true;
			break;

		case SDL_EVENT_KEY_DOWN:

			switch (event.key.key) {
			case SDLK_R:
				GenerateWorld();
				break;
			}

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				clickIsPressed = false;
				ticksToHold = ticksToHoldOG;
				/*for (int i = 0; i < sliders.size(); ++i)
				{
					sliders[i].Click(event.button.x, event.button.y, true);
				}*/
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {

				clickIsPressed = true;
				clickPressStartTime = SDL_GetTicks();
				accelerateTimer = 0;
				for (int i = 0; i < sliders.size(); ++i)
				{
					printf("Mouse Press %f, %f \n", event.button.x, event.button.y);
					sliders[i].Click(event.button.x, event.button.y, true);
				}
			}
			break;
		}
	}

	if (clickIsPressed)
	{
		if (SDL_GetTicks() > clickPressStartTime + ticksToHold)
		{
			clickPressStartTime = SDL_GetTicks();
			accelerateTimer++;

			if (accelerateTimer >= 5)
			{
				ticksToHold = ticksToHoldAccelerated;
			}

			for (int i = 0; i < sliders.size(); ++i)
			{
				float mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				sliders[i].Click(mouseX, mouseY, true);
			}
		}
	}

}

void Application::Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i].Render(renderer, cameraPosition, zoom);
	}

	// Draw texture if we have one
	if (mapTexture) {
		dst->x = cameraPosition.x;
		dst->y = cameraPosition.y;
		dst->w = zoom * width;
		dst->h = zoom * height;
		SDL_RenderTexture(renderer, mapTexture, nullptr, dst);
	}

	int windowW, windowH;
	SDL_GetWindowSize(window, &windowW, &windowH);

	for (int i = 0; i < sliders.size(); ++i)
	{
		sliders[i].position.x = windowW - 70;
		sliders[i].position.y = 50 * i;

		sliders[i].Render(renderer, window);
	}

	SDL_RenderPresent(renderer);
}

void Application::GetCoordinateFromIndex(int index, int width, int* x, int* y)
{
	*x = index % width;
	*y = index / width;	
}
