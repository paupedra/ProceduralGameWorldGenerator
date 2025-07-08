#pragma once
#include "Position.h"
#include "Entity.h"
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

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

	bool exit = false;

	float frameTime = 0.f;
	float appStartTime = 0.f;
	float frameRate = 60.f;
	int framesSinceStart = 0.f;
	int currentFrameTimeStart = 0.f;
	float lastFrameTimeDuration = 0.f;
	float lastFrameTime = 0.f;

	float dt = 0;

	float lastSecTime = 0.f;
	float lastSecTimeStart = 0.f;

	int lastSecFrames = 0.f;
	int prevLastSecFrames = 0.f;

	std::vector<Entity> entities;
	float tileSize = 5;

	////Matrix matrix(3, 3);
	//int matrix[10][10] = 
	//{	{1,0,0,0,0,0,0,0,0,0} , 
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//	{0,0,0,0,0,0,0,0,0,0} ,
	//};

	Position cameraPosition;

private:
	Application() = default; // no public constructor
	~Application() = default; // no public destructor
	static Application* instance; // declaration class variable
};

