#pragma once
#include "Position.h"
#include "Color.h"
#include "UIElement.h"
#include <vector>

constexpr int WIDTH = 200;
constexpr int HEIGHT = 200;

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_FRect;
struct TTF_Font;

struct WorldInfoTopView2D;

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

	void InitUISliders(SDL_Renderer* renderer);
	void GenerateWorld();
	void PollEvents();
	void Render();

	void GetCoordinateFromIndex(int index, int width, int *x, int *y);

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* mapTexture = nullptr;
	SDL_FRect* mapTextureDst = nullptr;

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

	float zoom = 2.f;
	float zoomSpeed = 0.25f;
	int cameraSpeed = 200;

	float width, height;
	float waterPercent;
	float generatorZoom;

	int clickPressStartTime = 0;
	int ticksToHold = 200;
	int ticksToHoldOG = 200;
	int ticksToHoldAccelerated = 75;
	bool clickIsPressed = false;
	int accelerateTimer = 0;

	Position cameraPosition;

	WorldInfoTopView2D* topViewWorld;

	int seed = 0;

	std::vector<UISlider> sliders;
	std::vector<UIButton> buttons;
	TTF_Font* font = nullptr;
	WorldInfoTopView2D* uiTopViewWorld;

	SDL_Texture* seedTexture = nullptr;
	SDL_Surface* seedSurface = nullptr;

private:
	Application() = default; // no public constructor
	~Application() = default; // no public destructor
	static Application* instance; // declaration class variable
};

