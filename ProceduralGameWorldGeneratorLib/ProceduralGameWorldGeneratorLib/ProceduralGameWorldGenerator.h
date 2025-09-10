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
	//int biomeId;
} Tile;

typedef struct BiomeTile
{
	int tileId;
} BiomeTile;

//typedef struct Biome
//{
//	int id;
//
//	//float biomeTerrainElevationVariation; //Specific Biome Elevation Variation
//} Biome;

typedef struct {
	float zoom;
	float weight;
	float variation;
} NoiseLayer;
// -- General Data Structs

// World Info Structs
typedef struct WorldInfoTopView2D
{
	Tile* tiles; //Output: Array of tiles present in the world

	int width, height; //Input: Width and Height of the map in tiles
	float zoom; //Input: Default to 1, sets the zoom for the noise, if the map is bigger a bigger zoom is recommended

	NoiseLayer* noiseLayers;

	bool assurePercentages; //Wether the generator should assure the percentage will be accurate but will require more processing time

	float waterPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water

	bool addBeach; //Wether the generator should generate beach
	float beachPercent; //In percentage from 0 to 99 the percentage of the land that should be beach sand
	
	//-- Future Implementations
	//Biome biomes[MAX_BIOMES]; //Input: Array of Biomes, to be set by the user
	//int biomeCount; //Set to 0

	//bool waterHasElevation; //Weather want to assign elevation to bodies of water
	//int maxTerrainElevation, minTerrainElevation; //In units maximum and minimum elevation in the whole world
	//int terrainElevationVariation; //How common it is to have more variation in elevation
} WorldInfoTopView2D;
// -- World Infos Structs


// --- FUNCTIONS ----

//External
bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config); //Validate Struct Before Generating World

void GenerateTopView2DWorld(WorldInfoTopView2D* info, int seed); //Generates 2D world from a Top View

//void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome);
//--External

//Internal
void AllocateTiles(WorldInfoTopView2D* info); //Allocates memory for tiles array

float ZoomablePerlinNoise3Seed(float zoom, float variation, float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed); //Calls Perlin and multiplies coordinates by zoom

//QuickSort
int CompareFloats(const void* a, const void* b);
void Swap(float* a, float* b); // Swap two float elements
int Partition(float arr[], int low, int high); // Partition the array and return the pivot index
void QuickSort(float arr[], int low, int high); // QuickSort function for floats
//--QuickSort

int RandomRange(int min, int max);
//--Internal

#ifdef __cplusplus
}
#endif