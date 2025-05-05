#pragma once
#include "Position.h"
#include "Entity.h"
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

class Application
{

public:
	bool Init();
	void PrepareUpdate();
	bool Update();
	void FinishUpdate();
	bool ShutDown();

	void GenerateWorld();
	void PollEvents();
	void Render();

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

	//Matrix matrix(3, 3);
	int matrix[10][10] = 
	{	{1,0,0,0,0,0,0,0,0,0} , 
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
		{0,0,0,0,0,0,0,0,0,0} ,
	};

	Position screenPos;
};

