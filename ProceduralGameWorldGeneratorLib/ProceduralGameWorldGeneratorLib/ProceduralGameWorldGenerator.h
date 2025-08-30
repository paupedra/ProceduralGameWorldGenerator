#pragma once
#include <stdbool.h>

#define MAX_BIOMES 16

#ifdef __cplusplus
extern "C" {
#endif

// --- STRUCTS ----

// General Data Structs
typedef struct Tile
{
	int tileId; //Represents the type of final tile
	int biomeId;
} Tile;

typedef struct BiomeTile
{
	int tileId;
} BiomeTile;

typedef struct Biome
{
	int id;


	//float biomeTerrainElevationVariation; //Specific Biome Elevation Variation
} Biome;

typedef struct {
	double zoom;
	double weight;
} NoiseLayer;
// -- General Data Structs

// World Info Structs
typedef struct WorldInfoTopView2D
{
	Tile* tiles; //Output: Array of tiles present in the world

	int width, height; //Input: Width and Height of the map in tiles
	float zoom; //Input: Default to 1, sets the zoom for the noise, if the map is bigger a bigger zoom is recommended

	NoiseLayer* noiseLayers;

	Biome biomes[MAX_BIOMES]; //Input: Array of Biomes, to be set by the user
	int biomeCount; //Set to 0

	bool assureWaterPercentage; //Wether the generator should assure the percentage will be accurate but will require more processing time
	float waterPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water

	bool addBeach;
	float beachPercent;
	
	//bool waterHasElevation; //Weather want to assign elevation to bodies of water
	//int maxTerrainElevation, minTerrainElevation; //In units maximum and minimum elevation in the whole world
	//int terrainElevationVariation; //How common it is to have more variation in elevation
} WorldInfoTopView2D;
// -- World Infos Structs


// --- FUNCTIONS ----

// Generation Functions
//External
bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config); //Validate Struct Before Generating World

void GenerateTopView2DWorld(WorldInfoTopView2D* info, int seed); //Generates 2D world from a Top View

void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome);
//--External

//Internal
void AllocateTiles(WorldInfoTopView2D* info);

float ZoomablePerlinNoise3Seed(float zoom, float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed);
//--Internal
// -- Generation Functions

//QuickSort
int CompareFloats(const void* a, const void* b);

void Swap(float* a, float* b); // Swap two float elements

int Partition(float arr[], int low, int high); // Partition the array and return the pivot index

void QuickSort(float arr[], int low, int high); // QuickSort function for floats
//--QuickSort

#ifdef __cplusplus
}
#endif