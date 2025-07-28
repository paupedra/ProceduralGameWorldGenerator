#pragma once
#include "Position.h"
#include "Entity.h"
#include "UISlider.h"
#include <vector>

constexpr int WIDTH = 1000;
constexpr int HEIGHT = 1000;

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_FRect;

class Application
{
public:
	// defines an class operation that lets clients access its unique instance.
	static Application& get() {
		// may be responsible for creating its own unique instance.
		if (nullptr == instance) instance = new Application;
		return *instance;
	}
	Application(const Application&) = delete; // rule of three
	Application& operator=(const Application&) = delete;
	static void destruct() {
		delete instance;
		instance = nullptr;
	}

	bool Init();
	void PrepareUpdate();
	bool Update();
	void FinishUpdate();
	bool ShutDown();

	void GenerateWorld();
	void PollEvents();
	void Render();

	void GetCoordinateFromIndex(int index, int width, int *x, int *y);

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* mapTexture = nullptr;
	SDL_FRect* dst;

	bool exit = false;

	float frameTime = 0.f;
	float appStartTime = 0.f;
	float frameRate = 60.f;
	int framesSinceStart = 0;
	int currentFrameTimeStart = 0;
	float lastFrameTimeDuration = 0.f;
	float lastFrameTime = 0.f;

	float dt = 0;

	float lastSecTime = 0.f;
	float lastSecTimeStart = 0.f;

	int lastSecFrames = 0;
	int prevLastSecFrames = 0;

	std::vector<Entity> entities;
	float tileSize = 5;
	float zoom = 0.5f;
	float zoomSpeed = 0.25f;
	int cameraSpeed = 200;

	Position cameraPosition;

	std::vector<UISlider> sliders;

private:
	Application() = default; // no public constructor
	~Application() = default; // no public destructor
	static Application* instance; // declaration class variable
};

