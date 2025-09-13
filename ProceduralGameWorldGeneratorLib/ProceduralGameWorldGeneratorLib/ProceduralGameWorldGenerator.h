#pragma once
#include <stdbool.h>

#define MAX_NOISE_LAYERS 10

#ifdef __cplusplus
extern "C" {
#endif

// --- STRUCTS ----

// General Data Structs
typedef struct Tile
{
	int tileId;
	int biomeId;
} Tile;
typedef struct Biome
{
	int id;
	int tileId;
} Biome;

typedef enum BiomeHeight
{
	ELEV_COUNT = 3,
	ELEV_HIGH = 2,
	ELEV_MEDIUM = 1,
	ELEV_LOW = 0
} BiomeHeight;

typedef enum BiomeTemperature
{
	TEMP_COUNT = 3,
	TEMP_HIGH = 2,
	TEMP_MEDIUM = 1,
	TEMP_LOW = 0
} BiomeTemperature;

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

	NoiseLayer elevationNoiseLayers[MAX_NOISE_LAYERS];
	int elevationNoiseLayersCount; //INITIALIZE TO 0
	NoiseLayer biomeNoiseLayers[MAX_NOISE_LAYERS];
	int biomeNoiseLayersCount; //INITIALIZE TO 0

	bool assurePercentages; //Wether the generator should assure the percentage will be accurate but will require more processing time

	float lowElevationPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water
	float midElevationPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water

	float lowTempPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water
	float midTempPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water

	bool addBeach; //Wether the generator should generate beach
	float beachPercent; //In percentage from 0 to 99 the percentage of the land that should be beach sand
	
	Biome biomes[ELEV_COUNT][TEMP_COUNT]; //Input: Array of Biomes, to be set by the user

} WorldInfoTopView2D;
// -- World Infos Structs


// --- FUNCTIONS ----

//External
bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config); //Validate Struct Before Generating World

void GenerateTopView2DWorld(WorldInfoTopView2D* info, int seed); //Generates 2D world from a Top View

void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome, BiomeHeight height, BiomeTemperature temperature);

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
//--Internal

#ifdef __cplusplus
}
#endif