#pragma once
#include <stdbool.h>

#define MAX_BIOMES 16

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Tile
{
	int tileId; //Represents the type of final tile
	int r, g, b; //Used in testing, will usually be a texture
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

typedef struct WorldInfoTopView2D
{
	Tile** tiles; //Output: Array of tiles present in the world
	Biome biomes[MAX_BIOMES]; //Input: Array of Biomes, to be set by the user
	int biomeCount; //Set to 0

	int width, height; //Input: Width and Height of the map in tiles
	int zoom; //Input: Default to 1, sets the zoom for the noise, if the map is bigger a bigger zoom is recommended

	bool assureWaterPercentage; //Wether the generator should assure the percentage will be accurate but will require more processing time
	float waterPercent; //In percentage from 0 to 99 the percentage of the world which will be bodies of water

	bool addBeach;
	float beachPercent;
	
	//bool waterHasElevation; //Weather want to assign elevation to bodies of water
	//int maxTerrainElevation, minTerrainElevation; //In units maximum and minimum elevation in the whole world
	//int terrainElevationVariation; //How common it is to have more variation in elevation
} WorldInfoTopView2D;

typedef struct {
	double zoom;
	double weight;
} NoiseLayer;

bool WorldInfoTopView2D_Validate(const WorldInfoTopView2D* config);

void AddBiome(WorldInfoTopView2D* worldInfo, Biome biome);

void GenerateTopView2DWorld(WorldInfoTopView2D* info, int seed); //Generates 2D world from a Top View

float ZoomablePerlinNoise3Seed(float zoom, float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed);

void AllocateTiles(WorldInfoTopView2D* info);

#ifdef __cplusplus
}
#endif