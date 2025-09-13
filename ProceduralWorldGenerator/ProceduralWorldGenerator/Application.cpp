#include "Application.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "ProceduralGameWorldGeneratorLib/ProceduralGameWorldGenerator.h"

bool Application::Init()
{
	window = SDL_CreateWindow("Hello World", 1000, 600, SDL_WINDOW_RESIZABLE);
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

	int windowW, windowH;
	SDL_GetWindowSize(window, &windowW, &windowH);

	InitUISliders(renderer);

	mapTextureDst = new SDL_FRect();
	topViewWorld = new WorldInfoTopView2D();
	topViewWorld->tiles = nullptr;

	GenerateWorld();

	cameraPosition.x = (windowW / 2) - (zoom * width / 2);
	cameraPosition.y = (windowH / 2) - (zoom * height / 2);

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
	delete(uiTopViewWorld);
	delete(mapTextureDst);
	delete(topViewWorld->tiles);
	delete(topViewWorld);
	delete(font);
	sliders.clear();
	buttons.clear();
	SDL_DestroyWindow(window);
	return true;
}

void Application::InitUISliders(SDL_Renderer* renderer)
{
	uiTopViewWorld = new WorldInfoTopView2D;

	width = WIDTH;
	height = HEIGHT;
	uiTopViewWorld->lowElevationPercent = 50.f;
	uiTopViewWorld->midElevationPercent = 75.f;
	uiTopViewWorld->lowTempPercent = 20.f;
	uiTopViewWorld->midTempPercent = 70.f;
	uiTopViewWorld->assurePercentages = true;
	uiTopViewWorld->addBeach = true;
	uiTopViewWorld->beachPercent = 20.f;
	uiTopViewWorld->zoom = 2.f;

	font = TTF_OpenFont("Assets/Fonts/console.ttf", 18);
	if (!font) {
		printf("Failed to load font: %s\n", SDL_GetError());
	}

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

	zoomSlider = new UISlider(&uiTopViewWorld->lowElevationPercent);
	zoomSlider->valueIncrements = 1;
	zoomSlider->minValue = 0.f;
	zoomSlider->maxValue = 99.f;
	zoomSlider->string = "Low Elev %";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&uiTopViewWorld->midElevationPercent);
	zoomSlider->valueIncrements = 1;
	zoomSlider->minValue = 0.f;
	zoomSlider->maxValue = 99.f;
	zoomSlider->string = "Mid Elev %";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&uiTopViewWorld->lowTempPercent);
	zoomSlider->valueIncrements = 1;
	zoomSlider->minValue = 0.f;
	zoomSlider->maxValue = 99.f;
	zoomSlider->string = "Low Temp %";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&uiTopViewWorld->midTempPercent);
	zoomSlider->valueIncrements = 1;
	zoomSlider->minValue = 0.f;
	zoomSlider->maxValue = 99.f;
	zoomSlider->string = "Mid Temp %";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&uiTopViewWorld->beachPercent);
	zoomSlider->valueIncrements = 1.f;
	zoomSlider->minValue = 0.f;
	zoomSlider->maxValue = 99.f;
	zoomSlider->string = "Beach %";
	sliders.push_back(*zoomSlider);

	zoomSlider = new UISlider(&uiTopViewWorld->zoom);
	zoomSlider->valueIncrements = 0.1f;
	zoomSlider->minValue = 1.1f;
	zoomSlider->maxValue = 100.f;
	zoomSlider->string = "GENZOOM";
	sliders.push_back(*zoomSlider);

	for (int i = 0; i < sliders.size(); ++i)
	{
		sliders[i].Init(renderer, font);
	}

	UIButton* button = new UIButton(&uiTopViewWorld->assurePercentages);
	button->string = "Assure Percentages";
	buttons.push_back(*button);

	button = new UIButton(&uiTopViewWorld->addBeach);
	button->string = "Add Beach";
	buttons.push_back(*button);

	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i].Init(renderer, font);
	}
}

void Application::GenerateWorld()
{
	topViewWorld->width = width;
	topViewWorld->height = height;
	topViewWorld->assurePercentages = uiTopViewWorld->assurePercentages;
	topViewWorld->lowElevationPercent = uiTopViewWorld->lowElevationPercent;
	topViewWorld->midElevationPercent = uiTopViewWorld->midElevationPercent;
	topViewWorld->lowTempPercent = uiTopViewWorld->lowTempPercent;
	topViewWorld->midTempPercent = uiTopViewWorld->midTempPercent;
	topViewWorld->zoom = uiTopViewWorld->zoom;
	topViewWorld->addBeach = uiTopViewWorld->addBeach;
	topViewWorld->beachPercent = uiTopViewWorld->beachPercent;

	Biome ocean      { 0, 0 }; 
	Biome desert     { 1, 1 };
	Biome frostPeak  { 2, 2 };
	Biome forest     { 3, 3 };
	Biome mountain   { 4, 4 };
	Biome glaciar    { 5, 5 };
	Biome hotMountain{ 6, 6 };
	Biome frostForest{ 7, 7 };
	Biome beach      { 8, 8 };

	AddBiome(topViewWorld, glaciar, BiomeHeight::ELEV_LOW, BiomeTemperature::TEMP_LOW);
	AddBiome(topViewWorld, ocean, BiomeHeight::ELEV_LOW, BiomeTemperature::TEMP_MEDIUM);
	AddBiome(topViewWorld, ocean, BiomeHeight::ELEV_LOW, BiomeTemperature::TEMP_HIGH);

	AddBiome(topViewWorld, frostForest, BiomeHeight::ELEV_MEDIUM, BiomeTemperature::TEMP_LOW);
	AddBiome(topViewWorld, forest, BiomeHeight::ELEV_MEDIUM, BiomeTemperature::TEMP_MEDIUM);
	AddBiome(topViewWorld, desert, BiomeHeight::ELEV_MEDIUM, BiomeTemperature::TEMP_HIGH);

	AddBiome(topViewWorld, frostPeak, BiomeHeight::ELEV_HIGH, BiomeTemperature::TEMP_LOW);
	AddBiome(topViewWorld, mountain, BiomeHeight::ELEV_HIGH, BiomeTemperature::TEMP_MEDIUM);
	AddBiome(topViewWorld, hotMountain, BiomeHeight::ELEV_HIGH, BiomeTemperature::TEMP_HIGH);

	seed = SDL_rand(10000000000);

	GenerateTopView2DWorld(topViewWorld, seed);

	std::vector<Uint32> pixels(topViewWorld->width * topViewWorld->height);// Create some pixel data (RGBA 32-bit format)

	Color color;
	for (int y = 0; y < topViewWorld->height; ++y)
	{
		for (int x = 0; x < topViewWorld->width; ++x)
		{
			if (topViewWorld->tiles)
			{
				//Color color;
				switch (topViewWorld->tiles[y * topViewWorld->width + x].tileId)
				{
					case 0: //Ocean
						color = Color(0,0,200,255); 
						break;
					case 1: //Desert
						color = Color(236,236,60, 255);
						break;
					case 2: //Frost Peak
						color = Color(93, 134, 171, 255);
						break;
					case 3: //Forest
						color = Color(67, 199, 58, 255);
						break;
					case 4: //Mountain
						color = Color(152, 152, 152, 255);
						break;
					case 5: //Glaciar
						color = Color(176, 237, 255, 255);
						break;
					case 6: //Hot Mountain
						color = Color(196, 98, 0, 255);
						break;
					case 7: //Frost Forest
						color = Color(61, 223, 142, 255);
						break;
					case 8: //Beach
						color = Color(255, 249, 156, 255);
						break;
				}
			}

			pixels[y * topViewWorld->width + x] = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
		}
	}
	
	mapTextureDst->x = 0;
	mapTextureDst->y = 0;
	mapTextureDst->w = topViewWorld->width;
	mapTextureDst->h = topViewWorld->height;

	if(mapTexture)
		SDL_DestroyTexture(mapTexture);

	mapTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		topViewWorld->width, topViewWorld->height);

	if (!mapTexture) {
		SDL_Log("Texture creation failed: %s", SDL_GetError());
	}
	else {
		if (!SDL_UpdateTexture(mapTexture, nullptr, pixels.data(), topViewWorld->width * sizeof(Uint32)) != 0) { // Update texture with our pixel data
			SDL_Log("Texture update failed: %s", SDL_GetError());
		}
	}

	SDL_DestroySurface(seedSurface);
	SDL_DestroyTexture(seedTexture);

	//Init Seed String
	SDL_Color seedColor = { 255,255,255,255 };
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "Seed: %d", seed);
	const char* str = buffer;
	seedSurface = TTF_RenderText_Solid(font, str, 0, seedColor);
	seedTexture = SDL_CreateTextureFromSurface(renderer, seedSurface);
	if (seedTexture == NULL) {
		SDL_Log("Couldn't create seed texture: %s", SDL_GetError());
	}
	
	pixels.clear();
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
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {

				clickIsPressed = true;
				clickPressStartTime = SDL_GetTicks();
				accelerateTimer = 0;
				for (int i = 0; i < sliders.size(); ++i)
				{
					sliders[i].Click(event.button.x, event.button.y, true,renderer);
				}

				for (int i = 0; i < buttons.size(); ++i)
				{
					buttons[i].Click(event.button.x, event.button.y,true, renderer);
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
				sliders[i].Click(mouseX, mouseY, true,renderer);
			}
		}
	}

}

void Application::Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int windowW, windowH;
	SDL_GetWindowSize(window, &windowW, &windowH);

	// Draw map texture if we have one
	if (mapTexture) {
		mapTextureDst->x = cameraPosition.x;
		mapTextureDst->y = cameraPosition.y;
		mapTextureDst->w = zoom * width;
		mapTextureDst->h = zoom * height;
		SDL_RenderTexture(renderer, mapTexture, nullptr, mapTextureDst);
	}

	SDL_FRect dest = { 0, 0, seedSurface->w, seedSurface->h };
	SDL_RenderTexture(renderer, seedTexture, NULL, &dest);

	float posY = 0;

	for (int i = 0; i < sliders.size(); ++i)
	{
		sliders[i].position.x = windowW - 70;
		sliders[i].position.y = posY;
		posY += 40;

		sliders[i].Render(renderer, window);
	}

	for (int i = 0; i < buttons.size(); ++i)
	{
		buttons[i].position.x = windowW - 70;
		buttons[i].position.y = posY;
		posY += 20;

		buttons[i].Render(renderer, window);
	}

	SDL_RenderPresent(renderer);
}

void Application::GetCoordinateFromIndex(int index, int width, int* x, int* y)
{
	*x = index % width;
	*y = index / width;	
}
